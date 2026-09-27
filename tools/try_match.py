#!/usr/bin/env python3
"""Compile a C file with the project toolchain and compare every function in it
byte-wise against SLUS_013.28, with relocated fields masked.

usage: tools/try_match.py [--psyq|--gcc28|--nocse] draft.c [func ...]

--psyq builds like src/main/psyq.c: GCC 2.7.2 -O2 and tools/aspsx_reorder.py.
--gcc28 builds like the PsyQ objects that came from GCC 2.8.1
-mno-split-addresses (tools/unfill_epilogue.py before maspsx).
--nocse builds like the PsyQ objects marked nocse: --psyq plus
-fno-rerun-cse-after-loop.

Functions that differ are printed side by side (ours | original) with the
differing instructions marked with **.
"""
import sys,subprocess,struct,re,os,tempfile
from elftools.elf.elffile import ELFFile
D=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
exe=open(f'{D}/disks/us/SLUS_013.28','rb').read()[0x800:]
args=sys.argv[1:]
gcc28='--gcc28' in args
nocse='--nocse' in args
psyq='--psyq' in args or gcc28 or nocse
args=[a for a in args if a not in ('--psyq','--gcc28','--nocse')]
src=args[0]; want=set(args[1:])
seg='psyq' if psyq else 'game'
w=os.path.join(tempfile.mkdtemp(prefix='try_match_'),'draft')
pre=""
if gcc28:
    cc1=f"{D}/bin/gcc-2.8.1-psx/cc1 -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -msoft-float -fgnu-linker -fsigned-char -fno-builtin -fdollars-in-identifiers -Wall -Wno-unused -mno-split-addresses"
    pre=f"python3 {D}/tools/unfill_epilogue.py < {w}.s |"
    post=f"| python3 {D}/tools/aspsx_reorder.py"
elif psyq:
    cc1=f"{D}/bin/gcc-2.7.2-psx/cc1 -quiet -O2 -G0 -mips1 -mcpu=3000 -mgas -msoft-float -fgnu-linker -fsigned-char -fno-builtin -fdollars-in-identifiers -Wall -Wno-unused"
    if nocse: cc1+=" -fno-rerun-cse-after-loop"
    post=f"| python3 {D}/tools/aspsx_reorder.py"
else:
    cc1=f"{D}/bin/gcc-2.95.2-psx/cc1 -quiet -O1 -G0 -mips1 -mcpu=3000 -mgas -msoft-float -fgnu-linker -Wall -Wno-unused"
    post=""
cmd=f"mipsel-linux-gnu-cpp -P -undef -nostdinc -I{D}/include -I{D}/external/psyq_headers/psyq_lib47/include -D_LANGUAGE_C -DLANGUAGE_C -D__GNUC__=2 -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D_MIPSEL -DSKIP_ASM {src} > {w}.i && {cc1} -o {w}.s {w}.i && {pre or f'cat {w}.s |'} python3 {D}/external/maspsx/maspsx.py --aspsx-version=2.86{" --expand-div" if psyq else ""} {post} > {w}.ms.s && mipsel-linux-gnu-as -EL -march=r3000 -no-pad-sections -O1 -G0 -o {w}.o {w}.ms.s"
r=subprocess.run(cmd,shell=True,capture_output=True,text=True)
if r.returncode: print(r.stderr); sys.exit(1)
if r.stderr.strip(): print(r.stderr.strip())
e=ELFFile(open(w+'.o','rb'))
text=e.get_section_by_name('.text').data()
rel={}
rs=e.get_section_by_name('.rel.text')
if rs: rel={x['r_offset']:x for x in rs.iter_relocations()}
symtab=e.get_section_by_name('.symtab')
syms=sorted([(s['st_value'],s.name) for s in symtab.iter_symbols() if s['st_info']['type']=='STT_FUNC'])
# Addresses of named symbols, to check that a relocated %lo/jal reaches the
# same address as the original's (the masked compare alone can't tell
# D_801D9638 from D_801D9680)
symaddr={}
try:
    for l in open(f'{D}/build/SLUS_013.28.map'):
        m=re.match(r'\s+0x([0-9a-f]{8})\s+([A-Za-z_]\w*)\s*$',l)
        if m: symaddr.setdefault(m.group(2),int(m.group(1),16))
except OSError: pass
def addr_of(n):
    m=re.match(r'(?:D|func|jtbl)_([0-9A-F]{8})$',n)
    return int(m.group(1),16) if m else symaddr.get(n)
def reloc_bad(o,word,line):
    """True if the relocation at o points elsewhere than the original's operand."""
    r=rel[o]; t=r['r_info_type']
    n=symtab.get_symbol(r['r_info_sym']).name
    want=re.findall(r'%lo\((\w+)\)',line) if t==6 else re.findall(r'^jal\s+(\w+)',line) if t==4 else []
    if not n or not want: return False
    a,b=addr_of(n),addr_of(want[0])
    if a is None or b is None: return n!=want[0]
    if t==6: a+=(word&0xffff)-((word&0x8000)<<1)
    return a!=b
dis=subprocess.run(['mipsel-linux-gnu-objdump','-d','--no-show-raw-insn',w+'.o'],capture_output=True,text=True).stdout
mine={}
for l in dis.splitlines():
    m=re.match(r'\s+([0-9a-f]+):\s+(.*)',l)
    if m: mine[int(m.group(1),16)]=re.sub(r'\s+',' ',m.group(2)).strip()
for i,(off,name) in enumerate(syms):
    if want and name not in want: continue
    m=re.match(r'func_([0-9A-F]{8})',name)
    asm=f'{D}/asm/main/nonmatchings/{seg}/{name}.s'
    if not os.path.exists(asm): print(name,'?'); continue
    t=open(asm).read()
    size=int(re.search(r'nonmatching \w+, 0x([0-9A-F]+)',t).group(1),16)
    addr=int(re.search(r'glabel '+name+r'\n\s+/\* [0-9A-F]+ ([0-9A-F]{8}) ',t).group(1),16)
    end=syms[i+1][0] if i+1<len(syms) else len(text)
    body=t[t.index('glabel '+name):] if 'glabel '+name in t else t
    tl=[re.sub(r'\s+',' ',re.sub(r'.*\*/\s+','',l)).strip() for l in body.splitlines() if re.match(r'\s+/\*',l)]
    nd=0; rows=[]
    for k in range(max(size,end-off)//4):
        o=off+4*k
        a=struct.unpack('<I',text[o:o+4])[0] if o<end else None
        b=struct.unpack('<I',exe[addr-0x80010000+4*k:][:4])[0] if 4*k<size else None
        rbad=False
        if a is not None and b is not None and o in rel:
            rbad=k<len(tl) and reloc_bad(o,a,tl[k])
            mk=0xfc000000 if (a>>26) in (2,3) else 0xffff0000; a&=mk; b&=mk
        bad=a!=b or rbad; nd+=bad
        rows.append(f"{'**' if bad else '  '} {mine.get(o,'') if o<end else '':38s}| {tl[k] if k<len(tl) else ''}")
    print(f'{name}: {"MATCH" if nd==0 else str(nd)+" diffs"} (size {end-off:#x} vs {size:#x})')
    if nd: print('\n'.join(rows))
