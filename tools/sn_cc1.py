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

It had no post-reload CSE either: it loads a constant again where ours copies
a register that already holds it (`li $a0,3` for ResetGraph(3) after a
compare with 3 in func_80061958), so reload_cse_regs() returns at once.

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

    patches = [(offset('mips_can_use_return_insn'), b'\x31\xc0\xc3'),
               (offset('reload_cse_regs'), b'\xc3'),
               (offset('mips_expand_epilogue') + 180, blk)]
data = bytearray(open(src, 'rb').read())
for off, code in patches:
    data[off:off + len(code)] = code
os.makedirs(os.path.dirname(dst) or '.', exist_ok=True)
with open(dst + '.tmp', 'wb') as f:
    f.write(data)
shutil.copymode(src, dst + '.tmp')
os.replace(dst + '.tmp', dst)
