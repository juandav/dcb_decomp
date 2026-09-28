#!/usr/bin/env python3
"""Extract one file from a .DRV drive file (such as P.DRV, which holds the
overlays).

usage: extract_drv.py DRIVE NAME OUT

A drive starts with a one-sector directory of 32-byte entries: a key, the
file's sector and size, four more bytes, and the name (16 bytes, padded
with zeros).
"""
import struct
import sys

SECTOR = 0x800

drive, name, out = sys.argv[1:4]
data = open(drive, "rb").read()
for i in range(0, SECTOR, 32):
    _key, sector, size = struct.unpack_from("<iii", data, i)
    if data[i + 16:i + 32].split(b"\0")[0].decode("ascii", "replace") == name:
        open(out, "wb").write(data[sector * SECTOR:sector * SECTOR + size])
        break
else:
    sys.exit(f"{name} is not in {drive}")
