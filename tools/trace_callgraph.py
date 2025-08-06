#!/usr/bin/env python3
"""Construct a call graph for trace-related functions.

This utility builds upon :mod:`trace_index` by using the cscope
cross-referencer to discover which functions are invoked by a subset of
functions containing a trace token.  The resulting directed graph is
emitted in Graphviz ``dot`` format, enabling downstream rendering or
analysis with tools such as ``networkx`` or ``graphviz``.

Example
-------
    $ python tools/trace_callgraph.py --limit 5
    wrote trace_callgraph.dot

Prerequisites
-------------
* ``ripgrep``
* ``universal-ctags``
* ``cscope``
* ``networkx`` and ``pydot`` (Python packages)
* ``graphviz`` (for rendering the output DOT file)
"""

from __future__ import annotations

from pathlib import Path
import argparse
import subprocess
import tempfile
import sys
from typing import Iterable

import networkx as nx

from trace_index import run_rg, assign_functions

ROOT = Path(__file__).resolve().parents[1]


def build_cscope(files: Iterable[Path], cwd: Path) -> None:
    """Generate a temporary cscope database for *files*."""
    filelist = cwd / "cscope.files"
    filelist.write_text("\n".join(str(f) for f in files))
    subprocess.run(
        ["cscope", "-b", "-q", "-i", str(filelist)],
        cwd=cwd,
        check=True,
    )


def calls_from(function: str, cwd: Path) -> list[tuple[str, str]]:
    """Return edges (function -> callee) using ``cscope -L -2``."""
    proc = subprocess.run(
        ["cscope", "-L", "-2", function],
        cwd=cwd,
        check=True,
        capture_output=True,
        text=True,
    )
    edges: list[tuple[str, str]] = []
    for line in proc.stdout.splitlines():
        parts = line.split()
        if len(parts) < 4:
            continue
        callee = parts[-1]
        edges.append((function, callee))
    return edges


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pattern", nargs="?", default="trace")
    parser.add_argument("--limit", type=int, default=10,
                        help="number of functions to analyse")
    parser.add_argument("--out", type=Path, default=Path("trace_callgraph.dot"),
                        help="output DOT file")
    args = parser.parse_args(argv)

    hits = run_rg(args.pattern)
    counts = assign_functions(hits)

    pairs = list(counts.items())[: args.limit]
    functions = [func for (_, func), _ in pairs]
    files = {ROOT / fname for (fname, _), _ in pairs}

    with tempfile.TemporaryDirectory() as tmpdir:
        cwd = Path(tmpdir)
        build_cscope(files, cwd)
        graph = nx.DiGraph()
        for func in functions:
            graph.add_node(func)
            for src, dst in calls_from(func, cwd):
                graph.add_edge(src, dst)
                graph.add_node(dst)
        nx.drawing.nx_pydot.write_dot(graph, args.out)

    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":  # pragma: no cover
    raise SystemExit(main())
