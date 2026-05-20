# profile.py
import time
import statistics
import argparse
import cirq
from benchmarks import BENCHMARKS
import qsimcirq

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

    runtimes = []
    for _ in range(args.repeat):
        qsim_simulator = qsimcirq.QSimSimulator(options)
        start = time.time()
        _ = qsim_simulator.simulate(circuit)
        runtimes.append(time.time() - start)
    
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


