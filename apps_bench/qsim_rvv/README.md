# qsim_rvv

`qsim_rvv` is a RISC-V RVV implementation based on upstream [`quantumlib/qsim`](https://github.com/quantumlib/qsim). The upstream project provides the qsim and qsimh quantum circuit simulators, qsimcirq Python bindings, sample circuits, and benchmark infrastructure. This tree keeps the upstream documentation in [`README_qsim.md`](README_qsim.md). It is ported to RVV1.0 support.


## What This Repo Adds 

The RVV-facing pieces are mainly:

- `lib/simulator_rvv.h` and `lib/statespace_rvv.h`: RVV simulator and state-space implementation.
- `lib/sve2rvv.h` and `lib/sve_vector.hh`: helper layer used by the vector backend work.
- `apps/qsim_base_rvv.cc`: standalone qsim app using `SimulatorRVV`.
- `circuit/`: Python generated script for GHZ circuits.

The original upstream README is preserved as [`README_qsim.md`](README_qsim.md). Use it for general qsim and qsimcirq background.

## Build C++ Apps

Build all C++ apps from the repository root:

```bash
make qsim
```

This calls `make -C apps/ qsim` and produces app binaries with `.x` suffixes in `apps/`, for example:

```text
apps/qsim_base.x # autovectorized version
apps/qsim_base_rvv.x # manually port rvv1.0
apps/qsim_von_neumann.x
apps/qsim_amplitudes.x
apps/qsimh_base.x
apps/qsimh_amplitudes.x
```

## Run the RVV App

`apps/qsim_base_rvv.cc` accepts the same basic arguments as upstream `qsim_base`:

```bash
./apps/qsim_base_rvv.x -c circuits/circuit_q24 -t $threads -f $fused -v $verbosity
./apps/qsim_base_rvv.x -c circuits/circuit_q24 -t 1 -f 3 -v 3
```

## Contact

ruimins@kth.se


