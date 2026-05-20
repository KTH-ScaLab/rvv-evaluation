# GEMM RVV Build and Run

This directory builds the PRK OpenMP GEMM benchmark for RISC-V scalar and RVV auto-vectorized variants.

## Build

Run the build script from this directory:

```bash
./compiler.sh
```

By default, the script builds both `float` and `double` variants for both compiler families:

```text
build/clang/float/
build/clang/double/
build/gcc/float/
build/gcc/double/
```

Each directory contains the same six executables:

```text
gemm_nonvec
gemm_autovec
gemm_autovec_1
gemm_autovec_2
gemm_autovec_4
gemm_autovec_8
```

`gemm_nonvec` uses `-march=rv64gc` and disables auto-vectorization. `gemm_autovec` uses `-march=rv64gcv` without forcing LMUL. The numbered `gemm_autovec_<LMUL>` binaries force LMUL through `RVV_LMUL`.

## Build Options

Build only one precision:

```bash
PRECISION=float ./compiler.sh
PRECISION=double ./compiler.sh
```

Override the precision list explicitly:

```bash
PRECISIONS="float double" ./compiler.sh
```

Override the LMUL list:

```bash
LMULS="1 2 4 8" ./compiler.sh
```

Override output location:

```bash
BUILD_DIR=/tmp/gemm-build ./compiler.sh
```

The compiler commands can also be overridden:

```bash
CLANG_CC="clang -std=c11 -pthread -static" ./compiler.sh
GCC_CC="riscv64-unknown-linux-gnu-gcc -std=c11 -pthread -static" ./compiler.sh
```

## Manual Make Examples

Float precision uses `float=1`; double precision uses `float=0` or omits the flag because `gemm.c` defaults to `double`.

```bash
make gemm clang=1 nonvec=1 float=1
make gemm clang=1 nonvec=0 RVV_LMUL=4 float=1
make gemm clang=0 nonvec=0 RVV_LMUL=4 float=1
make gemm clang=1 nonvec=1 float=0
make gemm clang=0 nonvec=0 RVV_LMUL=4 float=0
```

## Run

`run.sh` only executes existing binaries. It does not build, clean, or modify source files.
