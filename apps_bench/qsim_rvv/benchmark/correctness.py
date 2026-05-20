# profile.py
import time
import statistics
import argparse
import cirq
from benchmarks import BENCHMARKS
import qsimcirq
import numpy as np
parser = argparse.ArgumentParser()
parser.add_argument('-b','--benchmark', choices=BENCHMARKS.keys(), required=True)
parser.add_argument('-q','--qubits', type=int, nargs='+', required=True)
parser.add_argument('-t','--threads', type=int, default=1)
parser.add_argument('-f','--fused_gate', type=int, default=2)
parser.add_argument('-v','--verbose', type=int, default=0)
parser.add_argument('-r','--repeat', type=int, default=5)

args= parser.parse_args()
circuit_fn = BENCHMARKS[args.benchmark]
time_mean = []
time_stdev = []

for n in args.qubits:
    qubits = cirq.LineQubit.range(n)
    circuit = circuit_fn(qubits)
    options = {'t': args.threads, 'f': args.fused_gate, 'v': args.verbose}

    cirq_simulator = cirq.Simulator()
    cirq_result = cirq_simulator.simulate(circuit)
    
    runtimes = []
    for _ in range(args.repeat):
        qsim_simulator = qsimcirq.QSimSimulator(options)
        start = time.time()
        qsim_result = qsim_simulator.simulate(circuit)
        runtimes.append(time.time() - start)

    # Check closeness
    if np.allclose(cirq_result.final_state_vector, qsim_result.final_state_vector, atol=1e-6):
        print("\n The state vectors match!")
    else:
        print("\n The state vectors do not match.")
    
    if args.repeat==1:
        time_mean.append(runtimes)
        time_stdev.append(0)
    else:
        time_mean.append(statistics.mean(runtimes))
        time_stdev.append(statistics.stdev(runtimes))

print(f"\nBenchmark: {args.benchmark}")
print(f"Qubits: {args.qubits}")
print(f"Mean Times: {time_mean}")
print(f"Stddev Times: {time_stdev}")
