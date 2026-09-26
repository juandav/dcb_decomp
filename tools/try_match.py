#!/usr/bin/env python3
"""try.py file.c [func...]: compile with the project toolchain, compare each function byte-wise
against SLUS_013.28 (relocated fields masked) and show a side-by-side diff for mismatches."""
import sys,subprocess,struct,re,os
from elftools.elf.elffile import ELFFile
D='/home/juandav/code/dcb_decomp'
exe=open(f'{D}/disks/us/SLUS_013.28','rb').read()[0x800:]
src=sys.argv[1]; want=set(sys.argv[2:])
hdr=f'{D}/include/common.h'
w='/tmp/claude-1000/-home-juandav-code-dw-decomp/d2b36c10-5503-4300-af75-672dcd3b6641/scratchpad/trybuild'
cmd=f"mipsel-linux-gnu-cpp -P -undef -I{D}/include -DSKIP_ASM {src} > {w}.i && {D}/bin/gcc-2.95.2-psx/cc1 -quiet -O1 -G0 -mips1 -mcpu=3000 -mgas -msoft-float -fgnu-linker -Wall -Wno-unused -o {w}.s {w}.i && python3 {D}/external/maspsx/maspsx.py --aspsx-version=2.86 < {w}.s > {w}.ms.s && mipsel-linux-gnu-as -EL -march=r3000 -no-pad-sections -O1 -G0 -o {w}.o {w}.ms.s"
r=subprocess.run(cmd,shell=True,capture_output=True,text=True)
if r.returncode: print(r.stderr); sys.exit(1)
if r.stderr.strip(): print(r.stderr.strip())
e=ELFFile(open(w+'.o','rb'))
text=e.get_section_by_name('.text').data()
rel={}
rs=e.get_section_by_name('.rel.text')
if rs: rel={x['r_offset']:x for x in rs.iter_relocations()}
syms=sorted([(s['st_value'],s.name) for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC'])
dis=subprocess.run(['mipsel-linux-gnu-objdump','-d','--no-show-raw-insn',w+'.o'],capture_output=True,text=True).stdout
mine={}
for l in dis.splitlines():
    m=re.match(r'\s+([0-9a-f]+):\s+(.*)',l)
    if m: mine[int(m.group(1),16)]=re.sub(r'\s+',' ',m.group(2)).strip()
for i,(off,name) in enumerate(syms):
    if want and name not in want: continue
    m=re.match(r'func_([0-9A-F]{8})',name)
    asm=f'{D}/asm/main/nonmatchings/game/{name}.s'
    if not os.path.exists(asm): print(name,'?'); continue
    t=open(asm).read()
    size=int(re.search(r'nonmatching \w+, 0x([0-9A-F]+)',t).group(1),16)
    addr=int(re.search(r'/\* [0-9A-F]+ ([0-9A-F]{8}) ',t).group(1),16)
    end=syms[i+1][0] if i+1<len(syms) else len(text)
    tl=[re.sub(r'\s+',' ',re.sub(r'.*\*/\s+','',l)).strip() for l in t.splitlines() if re.match(r'\s+/\*',l)]
    nd=0; rows=[]
    for k in range(max(size,end-off)//4):
        o=off+4*k
        a=struct.unpack('<I',text[o:o+4])[0] if o<end else None
        b=struct.unpack('<I',exe[addr-0x80010000+4*k:][:4])[0] if 4*k<size else None
        if a is not None and b is not None and o in rel:
            mk=0xfc000000 if (a>>26) in (2,3) else 0xffff0000; a&=mk; b&=mk
        bad=a!=b; nd+=bad
        rows.append(f"{'**' if bad else '  '} {mine.get(o,'') if o<end else '':38s}| {tl[k] if k<len(tl) else ''}")
    print(f'{name}: {"MATCH" if nd==0 else str(nd)+" diffs"} (size {end-off:#x} vs {size:#x})')
    if nd: print('\n'.join(rows))
