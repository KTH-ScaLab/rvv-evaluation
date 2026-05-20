# Load/Store Instruction Microbenchmarks

This folder contains generated RISC-V scalar and RVV load/store
microbenchmarks to measure the peak thoughput of one individual memory access instructions. Kernels are generated as C++ files with inline assembly, linked
with `driver.cpp`, and built into `.x` executables.

## Run

The executable file in ```build``` folder are compiled with GCC15 and LLVM CLANG21 from All current run-script groups.

Compare the performance of tailing elements from 1-32 via setvl and mask operations: 

 ```bash
./run/run_vector_tail.sh 
```
The peak throughput of non-uniform load instructions. vlse stride is set to 2, vle is continigous load with 1010... maskcode to realize the strided load.

 ```bash
./run/run_vector_strided.sh 
```
The peak throughput of uniform load instructions

 ```bash
./run/run_vector_contiguous.sh
```

## Generate

You can also generate custome microbenchmarks with other memory instruction types. Some exmpale to generate the kernel:

```bash
./gen/gen_scalar_fp.sh         # scalar load/store kernels
./gen/gen_vector_vle.sh        # contiguous vle/vse kernels
./gen/gen_vector_vle_vm.sh     # masked vle/vse kernels
./gen/gen_vector_vle_setvl.sh  # setvl sweep kernels
./gen/gen_vector_vlse.sh       # strided vlse/vsse kernels
```

Current generated kernel inventory:

```text
scalar load/store
vle/vse normal
vle/vse masked 1010
vlse/vsse

vle/vse setvl 1-32
vle/vse setvm, equivient to setvl 1-32
```
The generator writes files into `kernels/`. 

## Build

Build both Clang and GCC variants:

```bash
make
```

Build only one variant:

```bash
make clang
make gcc
```

Outputs are compiler-specific:

```text
build/clang21/*.x
build/gcc15/*.x
```


```


## Output

Each executable prints semicolon-separated timing data:

```text
Iterations
Total Inst
Total Ops
Inst/Iter
Ops/Iter
Seconds
GOps/sec
```

Parse collected logs with:

```bash
python3 extract.py <log-file> <repetition> <cols>
```

## Notes

- `make` builds both compilers; run scripts control execution.
- Normal and strided vector cases use `ta/ma`.
- Masked and setvl cases use `tu/mu`.
- Object files are separated under each compiler output directory.
