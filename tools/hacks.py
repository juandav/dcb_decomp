#!/usr/bin/env python3
"""Count the workarounds the matched C needs, by the markers they carry.

    tools/hacks.py                  the counts
    tools/hacks.py --list           every one, with its file, line and function
    tools/hacks.py --check README.md
                                    fail if README.md's badge or table differs

Every spot where the C only matches through a form natural C wouldn't take
carries a comment that says so (CONTRIBUTING.md, "Matching"). This finds them
in src/ and include/:

- fake matches: a comment that starts with "fake match:", the last resort,
  a form forced only for the codegen;
- unused frame locals: a local the code never uses, kept because the
  original's stack frame has room for it, marked with exactly
  /* unused, but it is in the original stack frame */
- form-dependent matches: a comment that says "match depends on", where the
  match needs one of several equivalent forms (a statement macro's
  do-while, an extra block, braces left out, a copy of a variable, a type,
  one version's own form of a loop);
- functions still in assembly: INCLUDE_ASM (and INCLUDE_RODATA) lines.

The README's badge shows the fake matches, then the other two kinds
together. The functions still in assembly are counted apart, and --list also
shows what is assembly without being a hack: the hand-written .s files, and
the data written as top-level asm (padding that the original objects have).
"""

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DIRS = ("src", "include")

FRAME_MARKER = "unused, but it is in the original stack frame"

# (key, its label in the README's table)
KINDS = (
    ("fake", "Fake matches"),
    ("frame", "Unused frame locals"),
    ("form", "Form-dependent matches"),
    ("asm", "Functions still in assembly"),
)
HACKS = ("frame", "form")

# strings and comments, in the order they come, so that neither a comment
# inside a string nor a quote inside a comment is taken for the other
TOKEN = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*', re.S)
INCLUDE_ASM = re.compile(r"^\s*INCLUDE_(?:ASM|RODATA)\s*\(\s*[^,()]*,\s*(\w+)\s*\)", re.M)
ASM_DATA = re.compile(r'^__asm__\s*\(\s*"\.section', re.M)
FUNC_NAME = re.compile(r"(\w+)\s*\([^;{}]*\)\s*$")
DIRECTIVE = re.compile(r"^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", re.M)
GLABEL = re.compile(r"^\s*glabel\s+(\w+)", re.M)


def comment_text(comment):
    """A comment's words, without its delimiters and leading asterisks."""
    if comment.startswith("//"):
        body = comment[2:]
    else:
        body = comment[2:-2]
    lines = [re.sub(r"^\s*\*(?!/)", "", line) for line in body.split("\n")]
    return " ".join(" ".join(lines).split())


def blank(match):
    """The match with everything but its newlines turned into spaces."""
    return re.sub(r"[^\n]", " ", match[0])


def functions(code):
    """{line: function} for each line inside a function's braces, and the
    first line of each function definition, from code without comments or
    strings."""
    owner = {}
    depth = 0
    current = None
    start = 0
    lines = code.split("\n")
    for n, line in enumerate(lines, 1):
        for i, c in enumerate(line):
            if c == "{":
                if depth == 0:
                    # the header is the text since the last top-level ; or }
                    head = " ".join(lines[start:n - 1] + [line[:i]])
                    head = head.split(";")[-1].split("}")[-1]
                    m = FUNC_NAME.search(head.strip())
                    current = m[1] if m and "=" not in head else None
                    if current:
                        owner[n] = current
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    current = None
                    start = n - 1
            elif c == ";" and depth == 0:
                start = n - 1
        if current:
            owner[n] = current
    return owner, lines


def function_at(owner, lines, line):
    """The function a marker at this line is in or, outside of one, the
    function right after it (a comment on top of a function or a macro)."""
    if line in owner:
        return owner[line]
    for n in range(line + 1, len(lines) + 1):
        if n in owner:
            return owner[n]
    return ""


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


def scan():
    """{kind: [(path, line, function, text)]}, and the hand-written assembly
    files and asm data statements."""
    found = {key: [] for key, _ in KINDS}
    asm_files = []
    asm_data = []
    for top in DIRS:
        for path in sorted((ROOT / top).rglob("*")):
            rel = path.relative_to(ROOT).as_posix()
            if path.suffix == ".s":
                labels = GLABEL.findall(path.read_text(errors="replace"))
                asm_files.append((rel, len(labels)))
                continue
            if path.suffix not in (".c", ".h"):
                continue
            text = path.read_text(errors="replace")
            tokens = list(TOKEN.finditer(text))
            # strings and comments blanked out, so that neither a brace nor
            # an INCLUDE_ASM inside one counts
            code = TOKEN.sub(blank, text)
            # and the preprocessor's lines, so that a macro's braces don't
            # look like a function's
            owner, lines = functions(DIRECTIVE.sub(blank, code))
            for m in tokens:
                if m[0][0] != "/":
                    continue
                words = comment_text(m[0])
                n = line_of(text, m.start())
                if words.startswith("fake match:"):
                    kind = "fake"
                elif words == FRAME_MARKER:
                    kind = "frame"
                elif "match depends on" in words:
                    kind = "form"
                else:
                    continue
                func = function_at(owner, lines, n)
                if kind == "frame":
                    words = text.split("\n")[n - 1].split("/*")[0].strip()
                found[kind].append((rel, n, func, words))
            for m in INCLUDE_ASM.finditer(code):
                found["asm"].append((rel, line_of(code, m.start()), m[1], ""))
            for m in ASM_DATA.finditer(text):
                asm_data.append((rel, line_of(text, m.start())))
    return found, asm_files, asm_data


def readme_counts(text):
    """The badge's two numbers and the table's count for each kind."""
    counts = {}
    m = re.search(r"img\.shields\.io/badge/fake%20matches%20%7C%20hacks-(\d+)%20%7C%20(\d+)-", text)
    if m:
        counts["badge fake"] = int(m[1])
        counts["badge hacks"] = int(m[2])
    for key, label in KINDS:
        m = re.search(r"^\|\s*" + re.escape(label) + r"\s*\|\s*([\d,]+)\s*\|", text, re.M)
        if m:
            counts[key] = int(m[1].replace(",", ""))
    return counts


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--list", action="store_true", help="list every one")
    parser.add_argument("--check", metavar="README", help="check the README's badge and table")
    args = parser.parse_args()

    found, asm_files, asm_data = scan()
    count = {key: len(found[key]) for key in found}
    hacks = sum(count[key] for key in HACKS)

    if args.list:
        for key, label in KINDS:
            print(f"{label} ({count[key]}):")
            for path, line, func, text in found[key]:
                where = f"{path}:{line}"
                print(f"  {where:48} {func:36} {text if key != 'asm' else ''}".rstrip())
            print()
        print(f"Hand-written assembly files ({len(asm_files)}):")
        for path, n in asm_files:
            print(f"  {path} ({n} function{'s' if n != 1 else ''})")
        print()
        print(f"Data written as top-level asm ({len(asm_data)}):")
        for path, line in asm_data:
            print(f"  {path}:{line}")
        print()

    for key, label in KINDS:
        print(f"{label}: {count[key]}")
    print(f"Badge: fake matches | hacks = {count['fake']} | {hacks}")

    if args.check:
        readme = Path(args.check).read_text()
        want = dict(count, **{"badge fake": count["fake"], "badge hacks": hacks})
        have = readme_counts(readme)
        labels = dict(KINDS, **{"badge fake": "the badge's fake matches", "badge hacks": "the badge's hacks"})
        errors = []
        for key in want:
            if key not in have:
                errors.append(f"{args.check}: no count for {labels[key]}")
            elif have[key] != want[key]:
                errors.append(f"{args.check}: {labels[key]} says {have[key]}, the source has {want[key]}")
        for e in errors:
            print(e)
        if errors:
            sys.exit(f"{args.check} is out of date: run tools/hacks.py and update it")
        print(f"{args.check} is up to date")


if __name__ == "__main__":
    main()
