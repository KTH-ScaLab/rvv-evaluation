# Benchmark
This guide explains how to run the qsim benchmark script 
## command format
`run_qsim_leq32` is running a circuit with less than or equal to 32 qubits.

`run_qsim_gt32.py` is running a circuit with greater than 32 qubits.

An example of a command to run: 
```
INSTR=3 python benchmark/run_qsim_gt32.py -b grover -q 32 -t 72 -f 3 -r 5
```

'INSTR' supports the selection of different backend:  
      - `INSTR=0` runs lib/simulator_avx512.h  
      - `INSTR=1` runs lib/simulator_avx.h  
      - `INSTR=2` runs lib/simulator_sse.h  
      - `INSTR=3` runs lib/simulator_sve.h  
      - `INSTR=4` runs lib/simulator_basic.h  

`-b <BENCHMARK>` has the option: grover, ghz, QRC, qft, qv.

`-q <QUBITS>` Number of qubits.

`-t <THREADS>` Number of threads.

`-f <FUSED>` Maximum number of fused gates.

`-r <repeatition>` repeatition times.

`run_multibench.sh` script can run these 5 benchmarks with various configurations.

