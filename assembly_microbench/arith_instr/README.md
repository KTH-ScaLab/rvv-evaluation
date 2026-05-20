# Arithmetic Instruction Microbenchmarks

This folder generates and runs small RISC-V scalar/RVV arithmetic instruction
microbenchmarks. Each benchmark is generated as a C++ file with inline assembly,
linked with `driver.cpp`, and built into a `.x` executable.

## Run

The executable file in ```build``` folder are compiled with GCC15 and LLVM CLANG21 from All current run-script groups.


Run scalar integer benchmarks:

```bash
./run/run_scalar_int.sh
```

Run scalar floating-point benchmarks:

```bash
./run/run_scalar_fp.sh
```

Run RVV integer benchmarks:

```bash
./run/run_vector_int.sh
```

Run RVV floating-point benchmarks:

```bash
./run/run_vector_fp.sh
```


## Generate Kernels

You can also generate custome microbenchmarks with other memory instruction types. Some exmpale to generate the kernel:

Generate scalar integer kernels:

```bash
./gen/gen_scalar_int.sh   # Generate scalar int arithmetic ins
./gen/gen_scalar_fp.sh    # Generate scalar fp arithmetic ins
./gen/gen_vector_int.sh   # Generate vector int arithmetic ins
./gen/gen_vector_fp.sh    # Generate vector fp arithmetic ins
```

The generator writes files into `kernels/`. For example:

```text
kernels/add_8_m1_ta_ma.cpp
kernels/vadd_vv_8_m1_ta_ma.cpp
```

## Build

Run `make` to build both compiler variants:

```bash
make
```

This builds:

```text
build/clang21/*.x
build/gcc15/*.x
```

To build only one compiler:

```bash
make clang
make gcc
```

The compiler settings live in:

```text
config_clang.mk
config_gcc.mk
```



## Output

Each executable prints semicolon-separated timing information, including:

```text
Iterations
Total Inst
Total Ops
Inst/Iter
Ops/Iter
Seconds
GOps/sec
```

`extract.py` can parse collected output logs and reshape the results:

```bash
python3 extract.py <log-file> <repetition> <cols>
```

## Notes

- `make` compiles both Clang and GCC variants, but running is controlled by the
  scripts in `run/`.
- The current benchmark set uses `m1`, `ta`, and `ma`.
- Build outputs are compiler-specific so Clang and GCC do not share object
  files.
