#!/usr/bin/env python3
"""Binary-patch bin/gcc-2.7.2-psx/cc1 into the cc1 the PsyQ libraries were built with.

usage: tools/patch_cc1.py [in out]   (default: bin/gcc-2.7.2-psx/cc1 ->
                                       build/tools/gcc-2.7.2-psx/cc1)

Differences between our GCC 2.7.2 build and the compiler of the PsyQ 4.7
libraries (which also need -mhard-float, see FLOAT_ABI in the Makefile).
1 and 2 show up together: a short loaded once and used both sign-extended
and raw (`lh` + `lhu` of the same field in the ROM), and a bogus
`addiu $sp,-8/-16` frame (`.frame ... vars=8/16`) with no stack accesses.

1. try_combine: when three insns (load HI, sll 16, sra 16) combine into a
   sign-extending load while the HImode load is still needed, ours rewrites the
   second load as a SUBREG of the extended value; the ROM keeps both loads
   (the "two independent SETs" split). The SIGN_EXTEND special case is
   skipped here.
2. regclass: the combined-away shift temporaries keep stale reference counts.
   With no costs recorded their preferred class came out as ST_REGS (FP
   condition codes, no SImode), so global.c couldn't place them and reload
   gave them stack slots. The best-class search now starts below ST_REGS,
   which the PsyQ code (no floating point) never uses.
3. reorg's mark_target_live_regs stops its forward scan at a conditional
   jump before looking at the jump's own delay slot, so a register set there
   (`beqz v1,L; move v0,zero`) still counts as live and a branch just before
   can't take `li v0,K` from its target (libpad func_8002184C). The patched
   scan first records what that jump and its delay slot use and set (they
   run on both paths), then stops. Returns and other jumps are unchanged.
4. reorg's fill_simple_delay_slots never fills the slot of an unconditional
   jump from its target (2.7.2 only fills it from the insns before the jump);
   the ROM does, like GCC 2.8: `j L; <first insn at L>` with the jump
   redirected past it (_SsSetControlChange's four `j default; sra a0,a0,16`,
   the early `move v0,zero` of SsUtKeyOff, _SsVmInit's branch layout).
5. find_best_addr: a `reg + const_int` address (`4(p)` with p holding &sym)
   stays as it is instead of being folded into the constant `sym+4`.
6. alter_reg: a pseudo spilled to the stack gets a slot aligned to its own
   mode (4 bytes for SImode) instead of BIGGEST_ALIGNMENT (8), like GCC 2.8's
   `inherent_size == total_size ? 0 : -1`. Two spilled pseudos then sit at
   0x5C/0x60 instead of 0x60/0x68 (libmcrd MemCardGetDirentry's frame).

The whole build matches with the patched cc1 (none of the functions that
already matched changes).
"""
import os
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
STOCK = f"{ROOT}/bin/gcc-2.7.2-psx/cc1"
PATCHED = f"{ROOT}/build/tools/gcc-2.7.2-psx/cc1"


def patch(src, dst):
    d = bytearray(open(src, "rb").read())
    phoff, = struct.unpack_from("<I", d, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", d, 0x2A)
    segs, phidx = [], []
    for i in range(phnum):
        t, off, va, _, fsz = struct.unpack_from("<5I", d, phoff + i * phentsize)
        if t == 1:
            segs.append((va, off, fsz))
            phidx.append(i)

    def fo(va):
        for v, o, s in segs:
            if v <= va < v + s:
                return va - v + o
        raise ValueError(hex(va))

    def put(va, old, new):
        o = fo(va)
        if d[o:o + len(old)] != bytes(old):
            sys.exit(f"patch_cc1: unexpected bytes at {va:#x}: {d[o:o + len(old)].hex()}")
        d[o:o + len(new)] = new

    # 1. try_combine+9186: `jne` after `cmp $SIGN_EXTEND` -> `jmp` to the same target
    va = 0x08128E9F
    o = fo(va)
    rel = int.from_bytes(d[o + 2:o + 6], "little", signed=True)
    put(va, b"\x0f\x85" + d[o + 2:o + 6],
        b"\xe9" + (rel + 1).to_bytes(4, "little", signed=True) + b"\x90")
    # 2. regclass+2986: `for (class = ALL_REGS - 1; ...)` -> start at MD_REGS (6)
    put(0x0814313C, b"\xbe\x07\x00\x00\x00", b"\xbe\x06\x00\x00\x00")

    # Patches 3 and 4 add code after the end of the text segment (the rest of
    # its last page is zero padding in the file) and grow the segment over it.
    ti = next(i for i, (v, o, s) in enumerate(segs) if v <= 0x08172000 < v + s)
    tva, toff, tsz = segs[ti]
    end = [tva + tsz]

    def append(build):
        """Place the code build(cave, jump, code) produces after the segment."""
        cave = (end[0] + 15) & ~15
        code = bytearray()

        def jump(opcode, target):  # opcode + rel32 to target
            code.extend(opcode)
            code.extend((target - (cave + len(code) + 4)).to_bytes(4, "little", signed=True))

        build(code, jump)
        o = cave - tva + toff
        if d[o:o + len(code)] != bytes(len(code)):
            sys.exit("patch_cc1: no room after the text segment")
        d[o:o + len(code)] = code
        end[0] = cave + len(code)
        for field in (16, 20):  # p_filesz, p_memsz
            struct.pack_into("<I", d, phoff + phidx[ti] * phentsize + field, end[0] - tva)
        return cave

    # 3. mark_target_live_regs+3810: the `jne` that leaves the forward scan for
    # a jump that is neither simple nor a return goes to new code: for a
    # conditional jump (SET of pc from IF_THEN_ELSE) set next = 0 and run the
    # loop's marking code (+3873), which then ends the scan; anything else
    # leaves it as before (+4121).
    def scan_cond_jump(code, jump):
        code += b"\x8b\x45\x88"                    # mov -0x78(%ebp),%eax  (this_jump_insn)
        code += b"\x8b\x40\x10"                    # mov 0x10(%eax),%eax   (PATTERN)
        code += b"\x66\x83\x38\x29"                # cmpw $SET,(%eax)
        jump(b"\x0f\x85", 0x081725A6)             # jne break
        code += b"\x8b\x40\x08"                    # mov 0x8(%eax),%eax    (SET_SRC)
        code += b"\x66\x83\x38\x3e"                # cmpw $IF_THEN_ELSE,(%eax)
        jump(b"\x0f\x85", 0x081725A6)             # jne break
        code += b"\xc7\x85\x5c\xff\xff\xff" + bytes(4)  # movl $0,-0xa4(%ebp)  (next = 0)
        jump(b"\xe9", 0x081724AE)                  # jmp to the marking code

    cave = append(scan_cond_jump)
    o = fo(0x0817246F)
    put(0x0817246F, b"\x0f\x85" + d[o + 2:o + 6],
        b"\x0f\x85" + (cave - (0x0817246F + 6)).to_bytes(4, "little", signed=True))

    # 4. fill_simple_delay_slots+2819 (`if (delay_list)` before
    # emit_delay_sequence): first, as GCC 2.8 does, fill an empty slot of an
    # unconditional jump from its target:
    #   if (GET_CODE (insn) == JUMP_INSN && slots_filled != slots_to_fill
    #       && simplejump_p (insn))
    #     delay_list = fill_slots_from_thread (insn, const_true_rtx,
    #         next_active_insn (JUMP_LABEL (insn)), 0, 1, 1,
    #         own_thread_p (JUMP_LABEL (insn), JUMP_LABEL (insn), 0), 0,
    #         slots_to_fill, &slots_filled);
    # Locals: insn -0x84, delay_list -0x78, slots_filled -0x80, slots_to_fill -0x8c.
    def fill_jump_from_target(code, jump):
        skip = []

        def jcc(opcode):  # forward branch to `done`, fixed up below
            code.extend(opcode)
            skip.append(len(code))
            code.extend(bytes(4))

        code += b"\x8b\x85\x7c\xff\xff\xff"        # mov -0x84(%ebp),%eax  (insn)
        code += b"\x66\x83\x38\x1c"                # cmpw $JUMP_INSN,(%eax)
        jcc(b"\x0f\x85")
        code += b"\x8b\x45\x80"                    # mov -0x80(%ebp),%eax
        code += b"\x3b\x85\x74\xff\xff\xff"        # cmp -0x8c(%ebp),%eax
        jcc(b"\x0f\x84")
        code += b"\xff\xb5\x7c\xff\xff\xff"        # push insn
        jump(b"\xe8", 0x080F6486)                  # call simplejump_p
        code += b"\x83\xc4\x04\x85\xc0"            # add $4,%esp; test %eax,%eax
        jcc(b"\x0f\x84")
        code += b"\x8b\x85\x7c\xff\xff\xff"        # mov insn,%eax
        code += b"\x8b\x40\x20"                    # mov 0x20(%eax),%eax   (JUMP_LABEL)
        code += b"\x6a\x00\x50\x50"                # push 0; push label; push label
        jump(b"\xe8", 0x08170F0F)                  # call own_thread_p
        code += b"\x83\xc4\x0c\x50"                # add $12,%esp; push own
        code += b"\x8b\x85\x7c\xff\xff\xff"        # mov insn,%eax
        code += b"\xff\x70\x20"                    # push JUMP_LABEL
        jump(b"\xe8", 0x080DBF36)                  # call next_active_insn
        code += b"\x83\xc4\x04\x59"                # add $4,%esp; pop %ecx (own)
        code += b"\x8d\x55\x80\x52"                # lea -0x80(%ebp),%edx; push (&slots_filled)
        code += b"\xff\xb5\x74\xff\xff\xff"        # push slots_to_fill
        code += b"\x6a\x00\x51"                    # push 0 (own_opposite); push own
        code += b"\x6a\x01\x6a\x01\x6a\x00\x50"    # push 1; push 1; push 0; push thread
        code += b"\xff\x35" + (0x082D3980).to_bytes(4, "little")  # push const_true_rtx
        code += b"\xff\xb5\x7c\xff\xff\xff"        # push insn
        jump(b"\xe8", 0x08173610)                  # call fill_slots_from_thread
        code += b"\x83\xc4\x28\x89\x45\x88"        # add $40,%esp; mov %eax,delay_list
        for f in skip:
            code[f:f + 4] = (len(code) - (f + 4)).to_bytes(4, "little", signed=True)
        code += b"\x83\x7d\x88\x00"                # cmpl $0,delay_list  (the replaced insns)
        jump(b"\x0f\x84", 0x08173331)             # je
        jump(b"\xe9", 0x081732EE)                  # jmp back

    cave = append(fill_jump_from_target)
    put(0x081732E8, bytes.fromhex("837d88007443"),
        b"\xe9" + (cave - (0x081732E8 + 5)).to_bytes(4, "little", signed=True) + b"\x90")

    # 5. find_best_addr: don't fold a `reg + const_int` address (e.g. `4(p)`
    #    with p a pseudo holding &sym) into a constant `sym+4`. GCC 2.8 only
    #    keeps a folded address when it is cheaper, and a small reg+offset is
    #    already the cheapest; the PsyQ cc1 behaved like that (libmcrd/libgs
    #    `addiu v1,s0,-4; sw v0,4(v1)`). The test `code == REG` before the
    #    fold jumps to a cave (the body of `trace`, only used by -mdebugb)
    #    that also skips PLUS with a CONST_INT second operand.
    cave, back_fold, back_skip = 0x081C96DA, 0x080FD1F3, 0x080FD221
    code = bytearray(
        b"\x66\x83\xf8\x34"      # cmp ax, REG
        b"\x74\x12"                # je skip
        b"\x66\x83\xf8\x41"      # cmp ax, PLUS
        b"\x75\x11"                # jne fold
        b"\x8b\x4d\xac"           # mov ecx, [ebp-0x54]   (addr)
        b"\x8b\x49\x08"           # mov ecx, [ecx+8]      (XEXP (addr, 1))
        b"\x66\x83\x39\x2f"      # cmp word [ecx], CONST_INT
        b"\x75\x05"                # jne fold
    )
    code += b"\xe9" + (back_skip - (cave + len(code) + 5)).to_bytes(4, "little", signed=True)
    code += b"\xe9" + (back_fold - (cave + len(code) + 5)).to_bytes(4, "little", signed=True)
    o = fo(cave)
    if d[o:o + 4] != b"\xf3\x0f\x1e\xfb":
        sys.exit("patch_cc1: unexpected bytes at trace")
    d[o:o + len(code)] = code
    site = 0x080FD1ED
    put(site, b"\x66\x83\xf8\x34\x74\x2e",
        b"\xe9" + (cave - (site + 5)).to_bytes(4, "little", signed=True) + b"\x90")

    # 6. alter_reg+348: `assign_stack_local (mode, total_size, -1)` for a
    #    pseudo with no slot to reuse -> align 0 (the mode's alignment).
    put(0x08161CD9, b"\x6a\xff", b"\x6a\x00")

    os.makedirs(os.path.dirname(dst), exist_ok=True)
    tmp = dst + ".tmp"
    with open(tmp, "wb") as f:
        f.write(d)
    os.chmod(tmp, 0o755)
    os.replace(tmp, dst)


def ensure():
    """Path of the patched cc1, (re)built when missing or stale."""
    me = os.path.abspath(__file__)
    if (not os.path.exists(PATCHED)
            or os.path.getmtime(PATCHED) < max(os.path.getmtime(STOCK), os.path.getmtime(me))):
        patch(STOCK, PATCHED)
    return PATCHED


if __name__ == "__main__":
    if len(sys.argv) == 3:
        patch(sys.argv[1], sys.argv[2])
    else:
        print(ensure())
