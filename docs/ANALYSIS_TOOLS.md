# Analysis Tool Reference

This guide captures installation methods, minimal configuration, and sample
invocations for the tooling used to study and instrument DiscoBSD.

## cloc
- **Install**: `sudo apt install cloc`
- **Config**: none
- **Run**: `cloc tools`

```text
Language                     files          blank        comment           code
C                              118           5809           5726          39112
C/C++ Header                    53           1314           2291           7073
...
SUM:                           219           7661           8376          48397
```

## lizard
- **Install**: `python -m pip install lizard`
- **Config**: none
- **Run**: `python -m lizard tools/trace_inventory.py`

```text
NLOC    Avg.NLOC  AvgCCN  Avg.token  function_cnt    file
     59      15.5     3.0       90.0         2     tools/trace_inventory.py
```

## cscope
- **Install**: `sudo apt install cscope`
- **Config**: `find usr.bin/printf -name '*.c' > cscope.files`
- **Run**: `cscope -b -i cscope.files`

## diffoscope
- **Install**: `python -m pip install diffoscope` + `sudo apt install file`
- **Config**: none
- **Run**: `diffoscope README.md README.md` *(no output; files identical)*

## valgrind
- **Install**: `sudo apt install valgrind`
- **Config**: none
- **Run**: `valgrind --tool=memcheck true`

```text
==6551== Memcheck, a memory error detector
==6551== All heap blocks were freed -- no leaks are possible
```

## cppcheck
- **Install**: `sudo apt install cppcheck`
- **Config**: none
- **Run**: `cppcheck usr.bin/printf`

```text
Checking usr.bin/printf/printf.c ...
```

## sloccount
- **Install**: `sudo apt install sloccount`
- **Config**: none
- **Run**: `sloccount usr.bin/printf`

```text
SLOC    Directory       SLOC-by-Language (Sorted)
310     printf          ansic=310
```

## flawfinder
- **Install**: `sudo apt install flawfinder`
- **Config**: none
- **Run**: `flawfinder usr.bin/printf`

```text
usr.bin/printf/printf.c:45:  [4] (format) printf:
  If format strings can be influenced by an attacker, they can be exploited
```

## gdb
- **Install**: `sudo apt install gdb`
- **Config**: none
- **Run**: `gdb --version`

```text
GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
```

## pylint
- **Install**: `python -m pip install pylint`
- **Config**: none
- **Run**: `python -m pylint tools/trace_inventory.py`

```text
Your code has been rated at 10.00/10
```

## flake8
- **Install**: `python -m pip install flake8`
- **Config**: none
- **Run**: `python -m flake8 tools`

```text
tools/trace_callgraph.py:30:1: F401 'sys' imported but unused
```

## mypy
- **Install**: `python -m pip install mypy`
- **Config**: none
- **Run**: `mypy tools`

```text
tools/build/autobuild.py:16: error: Missing parentheses in call to 'print'.
```

## semgrep
- **Install**: `python -m pip install semgrep`
- **Config**: none
- **Run**: `semgrep --config=p/ci tools`

```text
✅ Scan completed successfully.
```

## eslint
- **Install**: `npm install -g eslint`
- **Config**: requires `eslint.config.js`
- **Run**: `eslint tools` *(fails: configuration missing)*

## jshint
- **Install**: `npm install -g jshint`
- **Config**: none
- **Run**: `jshint tools` *(no output; no JavaScript files)*

## jscpd
- **Install**: `npm install -g jscpd`
- **Config**: none
- **Run**: `jscpd tools`

```text
Clone found (c):
 - tools/virtualmips/jz4740/jz4740_dev_uart.c [263:42 - 272:4] ...
```

## nyc
- **Install**: `npm install -g nyc`
- **Config**: none
- **Run**: `nyc --version`

```text
17.1.0
```

