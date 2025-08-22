# Build and Instrumentation Guide

This document replaces the former `setup.sh` script and describes how to
configure a Linux host to build and analyse DiscoBSD using modern LLVM
infrastructure and the Ninja build executor.

## Prerequisites

The commands below assume a Debian/Ubuntu host with `sudo` privileges.
Package versions may evolve; verify them with `--version` when required.

```sh
sudo apt update
sudo apt install -y \
    build-essential \
    ninja-build \
    clang lld llvm \
    clang-format clang-tidy \
    gdb valgrind \
    linux-tools-common linux-tools-generic \
    byacc flex libfuse-dev pkg-config mandoc
```

> `perf` is provided by the `linux-tools-*` packages.  A kernel-specific
> package may be necessary when the running kernel version does not match the
> `linux-tools-generic` release.

Byacc and Flex supply the parser and lexer generators required for
building the `kconfig` utility.  `mandoc` renders its manual page, while
the `libfuse-dev` and `pkg-config` packages enable the optional
FUSE-based `fsutil` tool.

To verify the parser toolchain, explicitly select the host's parser and
lexer generators and rebuild the configuration utility:

```sh
export YACC=byacc
export LEX=flex
make -C tools/kconfig clean all
```

Optional tooling used for static and dynamic analysis includes `ripgrep`,
`universal-ctags`, `cscope`, `doxygen`, `graphviz`, `cppcheck`, `sloccount`,
`flawfinder`, and a suite of linters outlined below.  Refer to `ANALYZE.MD`
for an exhaustive list and version references.

## Configuring LLVM and Ninja

Ninja is the preferred entry point.  Assuming a `build.ninja` file exists (e.g.
produced by CMake or a bespoke generator), invoke:

```sh
ninja
```

To generate debug-friendly artefacts with aggressive optimisation while
retaining backtraces:

```sh
CFLAGS="-O2 -g -fno-omit-frame-pointer" \
CXXFLAGS="-O2 -g -fno-omit-frame-pointer" \
LDFLAGS="-fuse-ld=lld" \
ninja
```

These flags leverage Clang and LLD for code generation and linking.  Adjust the
`BOARD` or target-specific variables as required for embedded deployments.

## Instrumentation Workflow

- **`clang-tidy`** – static analysis: `clang-tidy path/to/file.c --`.
- **`clang-format`** – style enforcement: `clang-format -i path/to/file.c`.
- **`gdb`** – source-level debugging of produced binaries.
- **`valgrind`** – dynamic analysis for memory errors: `valgrind ./a.out`.
- **`perf`** – performance profiling when matching kernel tools are available.

Refer to `ANALYZE.MD` for additional exploration utilities and their usage.

## Tool Installation Matrix

The following matrix summarises canonical installation avenues derived from
project documentation and package manager manuals.

```text
+-------------------+---------------------+--------------------------------------------------+
| Tool              | Installation Method | Example Command                                  |
+-------------------+---------------------+--------------------------------------------------+
| lizard            | pip                 | pip install lizard                               |
| cloc              | apt                 | sudo apt install cloc                            |
| cscope            | apt                 | sudo apt install cscope                          |
| diffoscope        | pip                 | pip install diffoscope                           |
| dtrace            | source build        | git clone https://github.com/dtrace4linux/linux.git && make -C linux && sudo make -C linux install |
| valgrind          | apt                 | sudo apt install valgrind                        |
| cppcheck          | apt                 | sudo apt install cppcheck                        |
| sloccount         | apt                 | sudo apt install sloccount                       |
| flawfinder        | apt                 | sudo apt install flawfinder                      |
| gdb               | apt                 | sudo apt install gdb                             |
| pylint            | pip                 | pip install pylint                               |
| flake8            | pip                 | pip install flake8                               |
| mypy              | pip                 | pip install mypy                                 |
| semgrep           | pip                 | pip install semgrep                              |
| eslint            | npm                 | npm install eslint                               |
| jshint            | npm                 | npm install jshint                               |
| jscpd             | npm                 | npm install jscpd                                |
| nyc               | npm                 | npm install nyc                                  |
+-------------------+---------------------+--------------------------------------------------+
```
