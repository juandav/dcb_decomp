#!/usr/bin/env python3
"""Re-encode the non-ASCII text in the string and character literals of a
preprocessed C file as Shift JIS (CP932).

    tools/sjis_escape.py input.i output.i

The sources are UTF-8, so that the Japanese version's text reads as text,
but the game stores it as Shift JIS. GCC knows nothing of multi-byte
encodings (a trail byte of 0x5C, as in "ソ", would be taken as the start of
an escape), so every non-ASCII character inside a literal becomes octal
escapes of its CP932 bytes. Everything outside literals is copied unchanged;
the input comes from cpp, so it has no comments. The Makefile runs it
between cpp and cc1 for the versions whose .mk sets TEXT_ENCODING.
"""

import sys


def escape_literal(text, path, line):
    out = []
    for ch in text:
        if ord(ch) < 0x80:
            out.append(ch)
            continue
        try:
            data = ch.encode("cp932")
        except UnicodeEncodeError:
            sys.exit(f"{path}:{line}: {ch!r} (U+{ord(ch):04X}) has no CP932 encoding")
        out.append("".join(f"\\{b:03o}" for b in data))
    return "".join(out)


def convert(src, path):
    out = []
    i = 0
    n = len(src)
    line = 1
    while i < n:
        c = src[i]
        if c == '"' or c == "'":
            j = i + 1
            while j < n and src[j] != c:
                if src[j] == "\\":
                    j += 1
                elif src[j] == "\n":
                    sys.exit(f"{path}:{line}: unterminated literal")
                j += 1
            if j >= n:
                sys.exit(f"{path}:{line}: unterminated literal")
            j += 1
            out.append(escape_literal(src[i:j], path, line))
        else:
            # up to the next literal
            j = i + 1
            while j < n and src[j] not in "\"'":
                j += 1
            out.append(src[i:j])
        line += src.count("\n", i, j)
        i = j
    return "".join(out)


def main():
    if len(sys.argv) != 3:
        sys.exit(f"usage: {sys.argv[0]} input.i output.i")
    with open(sys.argv[1], encoding="utf-8") as f:
        src = f.read()
    result = convert(src, sys.argv[1])
    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write(result)


if __name__ == "__main__":
    main()
