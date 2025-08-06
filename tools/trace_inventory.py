#!/usr/bin/env python3
"""Enumerate and quantify *trace* occurrences within the source tree.

This utility inspects the repository for textual matches to a user-supplied
pattern (default: ``trace``) using ripgrep.  For each file containing a match,
it reports the number of occurrences.  The output forms a lightweight index
that aids in exploring tracing infrastructure and its cross-cutting presence
throughout the codebase.

Example
-------
    $ python tools/trace_inventory.py
    share/examples/c/skeleton.c\t1
    ...

    $ python tools/trace_inventory.py ktrace

Prerequisites
-------------
* ``ripgrep`` must be installed and discoverable in ``PATH``.
"""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import subprocess
import sys

# Programmatic constants
ROOT = Path(__file__).resolve().parents[1]


def collect_matches(pattern: str) -> Counter[str]:
    """Run ripgrep for *pattern* and return occurrence counts per file."""
    result = subprocess.run(
        [
            "rg",
            "--no-heading",
            "--with-filename",
            "--ignore-case",
            "-o",
            pattern,
            str(ROOT),
        ],
        check=True,
        capture_output=True,
        text=True,
    )

    counts: Counter[str] = Counter()
    for line in result.stdout.splitlines():
        filename, *_ = line.split(":", 1)
        counts[filename] += 1
    return counts


def main(argv: list[str] | None = None) -> int:
    """Entry point for CLI usage."""
    args = argv if argv is not None else sys.argv[1:]
    pattern = args[0] if args else "trace"
    counts = collect_matches(pattern)

    for fname, occurrences in sorted(counts.items()):
        rel = Path(fname).relative_to(ROOT)
        print(f"{rel}\t{occurrences}")

    print(f"\nTotal files containing '{pattern}': {len(counts)}")
    return 0


if __name__ == "__main__":  # pragma: no cover - CLI entry
    raise SystemExit(main())
