import statistics
import argparse

# find the optimal fused gate for one architecture
def sythnetic_circuit(qubits,numvals=4):
    circuit = cirq.Circuit()
    n = len(qubits)
    active = [q for i, q in enumerate(qubits) if i >= numvals]
    for q in active:
        circuit.append(cirq.H(q))
        circuit.append(cirq.rx(np.random.uniform(0, 2*np.pi))(q))
        circuit.append(cirq.ry(np.random.uniform(0, 2*np.pi))(q))
        circuit.append(cirq.rz(np.random.uniform(0, 2*np.pi))(q))
    return circuit

import qsimcirq

parser = argparse.ArgumentParser()
parser.add_argument('-q','--qubits', type=int, required=True)
parser.add_argument('-t','--threads', type=int, default=1)

args= parser.parse_args()

fused_gate=[1,2,3,4,5,6]
optimal_fused_gate=1

for f in fused_gate:
    qubits = cirq.LineQubit.range(args.qubits)
    circuit = sythnetic_circuit(qubits)
    options = {'t': args.threads, 'f': f, 'v': 0}

    runtimes = []
    for _ in range(5):
        qsim_simulator = qsimcirq.QSimSimulator(options)
        start = time.time()
        _ = qsim_simulator.simulate_into_1d_array(circuit)
        runtimes.append(time.time() - start)

        time_mean=statistics.mean(runtimes)

        if f==1:
            time_min= time_mean
        elif time_mean < time_min:
            optimal_fused_gate=f
            time_min = time_mean

print(f"The optimal fused gate on current machine is  {optimal_fused_gate}")
