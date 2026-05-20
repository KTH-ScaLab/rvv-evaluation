# Darknet RVV Builds

This folder keeps the Darknet source plus RISC-V build wrappers for comparing
non-vectorized and auto-vectorized binaries.

## Build

Use `compiler.sh` to build the benchmark binaries:

```bash
./compiler.sh
```

The script builds both GCC and Clang variants and writes the executables under:

```text
build/gcc/
build/clang/
```

For each compiler, it produces:

```text
darknet_nonvec
darknet_autovec
darknet_autovec_1
darknet_autovec_2
darknet_autovec_4
darknet_autovec_8
```

`darknet_nonvec` disables vectorization. `darknet_autovec` enables RVV
auto-vectorization with the compiler default. `darknet_autovec_1`,
`darknet_autovec_2`, `darknet_autovec_4`, and `darknet_autovec_8` build
auto-vectorized binaries with the requested LMUL setting.

## Configs

Compiler-specific flags are kept in:

```text
config_clang.mk
config_gcc.mk
```

The Makefile selects a config with `CONFIG`:

```bash
make CONFIG=config_clang.mk OPENMP=1 nonvec=0
make CONFIG=config_gcc.mk OPENMP=1 nonvec=1
```

LMUL-specific builds pass `RVV_LMUL`:

```bash
make CONFIG=config_clang.mk OPENMP=1 nonvec=0 RVV_LMUL=4
make CONFIG=config_gcc.mk OPENMP=1 nonvec=0 RVV_LMUL=4
```

`compiler.sh` is the preferred entrypoint because it cleans intermediate files
between builds and moves the final `darknet` executable into the expected
`build/<compiler>/` path.
