# Trace Inventory Utility

This repository contains a small helper script, `trace_inventory.py`, that
surveys the source tree for textual occurrences of tracing-related terms.  The
script leverages the speed of [`ripgrep`](https://github.com/BurntSushi/ripgrep)
and outputs a count of matches per file.

```sh
python tools/trace_inventory.py            # search for the default pattern 'trace'
python tools/trace_inventory.py ktrace     # search for a custom token
```

The resulting table provides a starting point for exploring the distribution and
interconnectivity of tracing hooks, documentation, and related artifacts.

# Trace Index Utility

`trace_index.py` refines the inventory by mapping each match to the enclosing
function using [universal-ctags](https://ctags.io/).  This yields a granular view
of where tracing-related tokens reside within the code's structural boundaries.

```sh
python tools/trace_index.py            # default search for 'trace'
python tools/trace_index.py ktrace     # search for a custom token
```

# Trace Call Graph Utility

`trace_callgraph.py` uses the cscope cross-referencer to infer which functions
are invoked by a subset of functions that contain a trace token.  The resulting
Graphviz ``dot`` file can be visualised with tools like `dot` or imported into
Python via `networkx`.

```sh
python tools/trace_callgraph.py --limit 5
```

All utilities emit tabular or graph outputs suitable for feeding into further
analysis pipelines.

# Code Metrics Utilities

Quantitative metrics can highlight hotspots for deeper inspection.

```sh
cloc tools/*.py                     # lines of code per language
python -m lizard tools              # cyclomatic complexity per function
```

# AST Exploration

The [`tree-sitter`](https://tree-sitter.github.io/tree-sitter/) CLI can parse
source files into syntax trees for structural analysis.

```sh
tree-sitter parse some_file.c
```

# Documentation

Sphinx and Doxygen configuration files are available for generating API
documentation:

```sh
# HTML docs via Sphinx
sphinx-build -b html docs docs/_build

# Doxygen output (HTML by default)
doxygen tools/Doxyfile
```

The generated content is written beneath ``docs/`` and can be served locally or
published as needed.
