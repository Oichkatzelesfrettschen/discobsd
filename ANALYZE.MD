# Trace Analysis Log

This log enumerates all tooling installed or exercised while exploring tracing
facilities within the codebase.

## Environment Setup

The commands below configure the environment. Version numbers were verified
after installation.

### System packages (apt)

```sh
apt-get update
apt-get install -y --no-install-recommends \
    ripgrep \
    universal-ctags \
    cscope \
    graphviz \
    doxygen \
    cloc \
    diffoscope \
    global \
    bpftrace \
    bpfcc-tools
```

### Python packages (pip)

```sh
python -m pip install networkx matplotlib pydot sphinx lizard
```

### Node packages (npm)

```sh
npm install -g tree-sitter-cli
```

### Package versions

- `ripgrep 14.1.0`
- `universal-ctags 5.9.0`
- `cscope 15.9`
- `graphviz 2.43.0`
- `doxygen 1.9.8`
- `cloc 1.98`
- `diffoscope 259`
- `global 6.6.11`
- `bpftrace 0.20.2`
- `tree-sitter 0.25.8`
- `networkx 3.5`
- `matplotlib 3.10.5`
- `pydot 4.0.1`
- `sphinx 8.2.3`
- `lizard 1.17.31`

## Configuration Files

- **Sphinx**: `docs/conf.py`
  ```sh
  python -m sphinx -b html docs docs/_build
  ```
- **Doxygen**: `tools/Doxyfile`
  ```sh
  doxygen tools/Doxyfile
  ```

## Available Utilities

- `tools/trace_inventory.py` – count files containing a trace-related token.
- `tools/trace_index.py` – map each token occurrence to its enclosing function.
- `tools/trace_callgraph.py` – build a call graph for a subset of matching
  functions.
- `cloc` – report lines of code: `cloc tools/*.py`
- `lizard` – compute complexity metrics: `python -m lizard tools`

## Example Workflow

```sh
python tools/trace_inventory.py > trace_files.tsv
python tools/trace_index.py > trace_funcs.tsv
python tools/trace_callgraph.py --limit 5
cloc tools/*.py
python -m lizard tools
```

The generated `trace_callgraph.dot` can be rendered with Graphviz:

```sh
dot -Tsvg trace_callgraph.dot -o callgraph.svg
```

## Additional Tools to Explore

The following packages were not exercised but complement the tracing workflow:

- `perf`, `systemtap`, and `ftrace` for kernel-level tracing comparisons.
- `JupyterLab` for interactive notebooks.
- `D3.js` for web-based visualisation.
- `CodeQL` or `Joern` for property-graph mining of security-relevant paths.

