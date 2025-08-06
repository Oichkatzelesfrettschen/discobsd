#!/usr/bin/env python3
"""Map trace-token occurrences to their containing functions.

This tool extends :mod:`trace_inventory` by correlating each textual match of a
user-specified pattern (default ``trace``) with the function that contains it.
It relies on two external utilities:

* ``ripgrep`` for fast pattern searches.
* ``universal-ctags`` for language-aware function boundaries.

The output is a tab-separated table describing the distribution of matches per
``(file, function)`` pair, which can subsequently be imported into other tools
for graph construction or further analysis.
"""

from __future__ import annotations

from collections import Counter
from dataclasses import dataclass
from pathlib import Path
import json
import subprocess
import sys
from typing import Iterable

ROOT = Path(__file__).resolve().parents[1]


@dataclass
class Function:
    """Lightweight representation of a function span."""

    name: str
    start: int
    end: int

    def contains(self, line: int) -> bool:
        return self.start <= line <= self.end


def run_rg(pattern: str) -> dict[Path, list[int]]:
    """Return mapping of files to line numbers with matches for *pattern*."""

    result = subprocess.run(
        [
            "rg",
            "--no-heading",
            "--with-filename",
            "--line-number",
            "--ignore-case",
            "-o",
            pattern,
            str(ROOT),
        ],
        check=True,
        capture_output=True,
        text=True,
    )

    hits: dict[Path, list[int]] = {}
    for line in result.stdout.splitlines():
        fname, line_no, _ = line.split(":", 2)
        hits.setdefault(Path(fname), []).append(int(line_no))
    return hits


def run_ctags(fname: Path) -> list[Function]:
    """Extract function definitions for *fname* using universal-ctags."""

    proc = subprocess.run(
        [
            "ctags",
            "--output-format=json",
            "--fields=+n",
            "-o",
            "-",
            str(fname),
        ],
        check=True,
        capture_output=True,
        text=True,
    )

    tags = [json.loads(line) for line in proc.stdout.splitlines() if line]
    funcs: list[Function] = []
    for tag in tags:
        if tag.get("kind") != "function":
            continue
        line = int(tag["line"])
        funcs.append(Function(tag["name"], line, line))
    funcs.sort(key=lambda f: f.start)
    for i, func in enumerate(funcs):
        end = funcs[i + 1].start - 1 if i + 1 < len(funcs) else 10**9
        func.end = end
    return funcs


def assign_functions(hits: dict[Path, list[int]]) -> Counter[tuple[str, str]]:
    """Resolve each match to its containing function."""

    counter: Counter[tuple[str, str]] = Counter()
    for fname, lines in hits.items():
        funcs = run_ctags(fname)
        for line in lines:
            func_name = next(
                (f.name for f in funcs if f.contains(line)),
                "<global>",
            )
            rel = fname.relative_to(ROOT)
            counter[(str(rel), func_name)] += 1
    return counter


def main(argv: Iterable[str] | None = None) -> int:
    args = list(argv) if argv is not None else sys.argv[1:]
    pattern = args[0] if args else "trace"

    hits = run_rg(pattern)
    counts = assign_functions(hits)

    for (fname, func), occurrences in sorted(counts.items()):
        print(f"{fname}\t{func}\t{occurrences}")

    print(f"\nTotal (file, function) pairs containing '{pattern}': {len(counts)}")
    return 0


if __name__ == "__main__":  # pragma: no cover
    raise SystemExit(main())
