# SPMV-v2 RVV Build and Run

This directory builds the PRK OpenMP sparse matrix-vector benchmark for RISC-V scalar and RVV auto-vectorized variants.

## Build

Use `compile.sh` from this directory:

```bash
./compile.sh
```

The script builds both compiler families and writes executables under the local `build/` directory:

```text
build/clang/sparse_nonvec
build/clang/sparse_autovec
build/clang/sparse_autovec_1
build/clang/sparse_autovec_2
build/clang/sparse_autovec_4
build/clang/sparse_autovec_8
build/gcc/sparse_nonvec
build/gcc/sparse_autovec
build/gcc/sparse_autovec_1
build/gcc/sparse_autovec_2
build/gcc/sparse_autovec_4
build/gcc/sparse_autovec_8
```

`sparse_nonvec` uses `-march=rv64gc` and disables auto-vectorization. `sparse_autovec` uses `-march=rv64gcv` without forcing LMUL. The numbered `sparse_autovec_<LMUL>` binaries force LMUL through `RVV_LMUL`.

Clang builds use LLVM OpenMP via `-fopenmp`. Do not replace this with `-fopenmp=libgomp` for this toolchain: that links `libgomp` but does not lower `#pragma omp parallel`, so the program runs with one thread.

## Run

The sparse benchmark arguments are:

```text
<# threads> <# iterations> <2log grid size> <stencil radius> <repetition>
```

For Clang binaries, prefer the wrapper so optional LLVM OpenMP tooling and hidden helper threads are disabled in the static RISC-V setup:

```bash
./run.sh ./build/clang/sparse_autovec 8 10 8 10 1
```

Equivalent direct command:

```bash
OMP_TOOL=disabled LIBOMP_USE_HIDDEN_HELPER_TASK=FALSE LIBOMP_NUM_HIDDEN_HELPER_THREADS=0 ./build/clang/sparse_autovec 8 10 8 10 1
```

GCC binaries can be run directly:

```bash
./build/gcc/sparse_autovec 8 10 8 10 1
```

## Manual Make Examples

```bash
make sparse clang=1 nonvec=1
make sparse clang=1 nonvec=0 RVV_LMUL=4
make sparse clang=0 nonvec=0 RVV_LMUL=4
```
