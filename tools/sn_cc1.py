#!/usr/bin/env python3
"""Make the GCC 2.8.1 cc1 behave like the one that built some PsyQ objects.

That compiler never used MIPS `return` insns: every early return in a function
without a frame jumps to the one `j $31` at the end, as GCC 2.7.2 does
(_SsReadDeltaValue, func_8006D3C0). Our GCC 2.8.1 turns such a jump into a
second `j $31` whenever mips_can_use_return_insn() says yes, so this patches
that function to return 0 (x86: xor eax,eax; ret).

usage: sn_cc1.py cc1 patched_cc1
"""
import os, shutil, sys
from elftools.elf.elffile import ELFFile

src, dst = sys.argv[1], sys.argv[2]
with open(src, 'rb') as f:
    e = ELFFile(f)
    sym = e.get_section_by_name('.symtab').get_symbol_by_name('mips_can_use_return_insn')[0]
    va = sym['st_value']
    off = next(s['p_offset'] + va - s['p_vaddr'] for s in e.iter_segments()
               if s['p_type'] == 'PT_LOAD' and s['p_vaddr'] <= va < s['p_vaddr'] + s['p_filesz'])
data = bytearray(open(src, 'rb').read())
data[off:off + 3] = b'\x31\xc0\xc3'
os.makedirs(os.path.dirname(dst) or '.', exist_ok=True)
with open(dst + '.tmp', 'wb') as f:
    f.write(data)
shutil.copymode(src, dst + '.tmp')
os.replace(dst + '.tmp', dst)
