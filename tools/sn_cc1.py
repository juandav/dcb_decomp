#!/usr/bin/env python3
"""Make the GCC 2.8.1 cc1 behave like the one that built some PsyQ objects.

That compiler never used MIPS `return` insns: every early return in a function
without a frame jumps to the one `j $31` at the end, as GCC 2.7.2 does
(_SsReadDeltaValue, func_8006D3C0). Our GCC 2.8.1 turns such a jump into a
second `j $31` whenever mips_can_use_return_insn() says yes, so this patches
that function to return 0 (x86: xor eax,eax; ret).

Its epilogue was text, not RTL, so the scheduler could never move the
register restores up into the body. mips_expand_epilogue() only puts a
blockage before the restores when there is a frame pointer; this makes it do
so always (the frame pointer check follows the blockage), so the restores stay
at the end (_spu_init, _padInitDirPort).

For the same reason reorg saw no insns after the last one of the body: a
branch falling into the epilogue had end_of_function_needs as its live
registers. Ours scans into the RTL epilogue with stale flow info and keeps the
counter of a function-ending loop live, so it fills the loop branch's slot
with the counter increment plus an undoing `addiu -1` after the loop
(_spu_FiDMA, func_8004AC20). mark_target_live_regs() now gives a target inside
the epilogue the registers needed at the start of the epilogue (helper written
over iterator_loop_prologue, GNU C iterators being unused). The bare return
jump of a frameless function is left alone: a branch to it keeps its slot
empty (func_8006B584).

It had no post-reload CSE either: it loads a constant again where ours copies
a register that already holds it (`li $a0,3` for ResetGraph(3) after a
compare with 3 in func_80061958), so reload_cse_regs() returns at once.

Its MIPS I register set left the one FP condition code register usable, as
GCC 2.7.2 does; 2.8.1 fixes all eight, which leaves one register less in
loop.c's hoisting threshold (2 * (1 + non-fixed registers)), so the original
hoists loop invariants ours keeps in the loop (CD_ready's table addresses).
The CONDITIONAL_REGISTER_USAGE loop in init_reg_sets_1 now starts at $fcc1.

usage: sn_cc1.py cc1 patched_cc1
"""
import os, shutil, sys
from elftools.elf.elffile import ELFFile

src, dst = sys.argv[1], sys.argv[2]
with open(src, 'rb') as f:
    e = ELFFile(f)
    symtab = e.get_section_by_name('.symtab')
    segs = [s for s in e.iter_segments() if s['p_type'] == 'PT_LOAD']

    def offset(name):
        va = symtab.get_symbol_by_name(name)[0]['st_value']
        return next(s['p_offset'] + va - s['p_vaddr'] for s in segs
                    if s['p_vaddr'] <= va < s['p_vaddr'] + s['p_filesz'])

    def sym(name):
        return symtab.get_symbol_by_name(name)[0]['st_value']

    # mips_expand_epilogue+180 held `if (frame_pointer_needed) {
    # emit_insn (gen_blockage ()); ...`; rewrite those 29 bytes as
    # `emit_insn (gen_blockage ()); if (frame_pointer_needed) {`
    at = sym('mips_expand_epilogue') + 180
    raw = open(src, 'rb').read()
    # the operand of its `mov $frame_pointer_needed,%eax`
    fp_needed = raw[offset('mips_expand_epilogue') + 182:offset('mips_expand_epilogue') + 186]
    skip = at + 128  # mips_expand_epilogue+308: save_restore_insns call

    def rel32(a, t):
        return (t - (a + 5)).to_bytes(4, 'little', signed=True)

    blk = (b'\xe8' + rel32(at, sym('gen_blockage')) + b'\x50'
           + b'\xe8' + rel32(at + 6, sym('emit_insn')) + b'\x58'
           + b'\xb8' + fp_needed + b'\x8b\x00\x85\xc0')
    blk += b'\x74' + (skip - (at + len(blk) + 2)).to_bytes(1, 'little', signed=True)
    blk += b'\x90' * (29 - len(blk))

    # Helper for mark_target_live_regs, reading its target (-0xcc(%ebp)):
    # returns 0 for a null target (end of function), 1 to go on as usual;
    # for a non-jump insn of the epilogue it returns 0 with %ebx lowered by
    # 16, so the caller's copy of end_of_function_needs (0xcf60(%ebx)) reads
    # start_of_epilogue_needs instead (%ebx is popped on the way out).
    cave = sym('iterator_loop_prologue')
    live = b'\x8b\x85' + (-0xcc).to_bytes(4, 'little', signed=True)  # mov target,%eax
    live += b'\x85\xc0\x74\x2a'                                     # test; je ret0
    live += b'\x66\x83\x38\x1c\x74\x1e'                           # JUMP_INSN? je ret1
    live += b'\x8b\x15' + sym('epilogue').to_bytes(4, 'little')      # mov epilogue,%edx
    live += b'\x85\xd2\x74\x14\x52\x50'                           # test; je ret1; push
    live += b'\xe8' + rel32(cave + len(live), sym('contains'))        # call contains
    live += b'\x83\xc4\x08\x85\xc0\x74\x06'                       # pop; test; je ret1
    live += b'\x83\xeb\x10\x31\xc0\xc3'                           # ebx -= 16; return 0
    live += b'\xb8\x01\x00\x00\x00\xc3\x31\xc0\xc3'             # ret1: 1; ret0: 0
    # mark_target_live_regs+83: `if (target == 0)` -> `if (!helper ())`
    site = sym('mark_target_live_regs') + 83
    test = b'\xe8' + rel32(site, cave) + b'\x85\xc0\x75\x2e'
    # init_reg_sets_1+0xab: `cmp $3,%edx; jg; movl $ST_REG_FIRST,-0x10(%ebp)`
    fcc = offset('init_reg_sets_1') + 0xab
    assert raw[fcc:fcc + 12] == bytes.fromhex('83fa037f3cc745f043000000')

    patches = [(offset('mips_can_use_return_insn'), b'\x31\xc0\xc3'),
               (fcc + 8, b'\x44'),
               (offset('reload_cse_regs'), b'\xc3'),
               (offset('mips_expand_epilogue') + 180, blk),
               (offset('iterator_loop_prologue'), live),
               (offset('mark_target_live_regs') + 83, test)]
data = bytearray(open(src, 'rb').read())
for off, code in patches:
    data[off:off + len(code)] = code
os.makedirs(os.path.dirname(dst) or '.', exist_ok=True)
with open(dst + '.tmp', 'wb') as f:
    f.write(data)
shutil.copymode(src, dst + '.tmp')
os.replace(dst + '.tmp', dst)
