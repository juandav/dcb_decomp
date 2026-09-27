#!/usr/bin/env python3
"""Make the GCC 2.8.1 cc1 behave like the one that built some PsyQ objects.

That compiler never used MIPS `return` insns: every early return in a function
without a frame jumps to the one `j $31` at the end, as GCC 2.7.2 does
(_SsReadDeltaValue, func_8006D3C0). Our GCC 2.8.1 turns such a jump into a
second `j $31` whenever mips_can_use_return_insn() says yes, so this patches
that function to return 0 (x86: xor eax,eax; ret).

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

    patches = [(offset('mips_can_use_return_insn'), b'\x31\xc0\xc3'),
               (offset('reload_cse_regs'), b'\xc3')]
data = bytearray(open(src, 'rb').read())
for off, code in patches:
    data[off:off + len(code)] = code
os.makedirs(os.path.dirname(dst) or '.', exist_ok=True)
with open(dst + '.tmp', 'wb') as f:
    f.write(data)
shutil.copymode(src, dst + '.tmp')
os.replace(dst + '.tmp', dst)
