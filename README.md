# rvv-evaluation


RVV-Eval is a benchmark suite for studying performance portability on RISC-V Vector (RVV) processors. It contains three benchmark groups:

1. **Assembly microbenchmarks** for instruction-level throughput studies.
2. **Proxy benchmarks** for application kernels and larger benchmark codes.
3. **Applications** for end-to-end workloads ported to or evaluated on RVV.


## Group 1: Assembly Microbenchmarks

The assembly benchmarks generate small C++ kernels with inline assembly and measure low-level scalar/RVV instruction behavior.

- Arithmetic instructions: [`assembly_microbench/arith_instr/README.md`](assembly_microbench/arith_instr/README.md)
- Load/store instructions: [`assembly_microbench/ldst_instr/README.md`](assembly_microbench/ldst_instr/README.md)

## Group 2: Proxy Benchmarks

The proxy benchmarks compare non-vectorized, auto-vectorized, and LMUL-controlled RVV builds for larger kernels and applications.

- Darknet: [`proxy_bench/darknet/README.rvv.md`](proxy_bench/darknet/README.rvv.md)
- GEMM: [`proxy_bench/gemm/README.rvv.md`](proxy_bench/gemm/README.rvv.md)
- SPMV-v2: [`proxy_bench/spmv-v2/README.rvv.md`](proxy_bench/spmv-v2/README.rvv.md)

## Group 3: Applications

The application benchmarks contain larger end-to-end workloads evaluated or ported for RVV.

- qsim_rvv: [`apps_bench/qsim_rvv/README.md`](apps_bench/qsim_rvv/README.md)

## Citation

Please cite:

```bibtex
@misc{shi2026closergapportableperformance,
      title={Closer in the Gap: Towards Portable Performance on RISC-V Vector Processors}, 
      author={Ruimin Shi and Maya Gokhale and Pei-Hung Lin and Xavier Teruel and Ivy Peng},
      year={2026},
      eprint={2605.10860},
      archivePrefix={arXiv},
      primaryClass={cs.DC},
      url={https://arxiv.org/abs/2605.10860}, 
}
```

## Contact
Ruimin Shi: `ruimins@kth.se`.
