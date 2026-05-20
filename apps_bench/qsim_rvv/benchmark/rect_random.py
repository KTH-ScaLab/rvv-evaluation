import time
import cirq
import qsimcirq
import statistics
# Get a rectangular grid of qubits.
qubits = cirq.GridQubit.rect(1, 1)


# Generates a random circuit on the provided qubits.
circuit = cirq.experiments.random_rotations_between_grid_interaction_layers_circuit(
    qubits=qubits, depth=16)

options = {'cpu': 4}
time_mean1 = []
time_stdev1 =[]
tmp =[]
# Simulate the circuit with qsim and print the runtime.
for i in range(5):
    qsim_simulator = qsimcirq.QSimSimulator(options)
    qsim_start = time.time()
    qsim_results = qsim_simulator.simulate(circuit)
    tmp.append(time.time() - qsim_start)

time_mean1.append(statistics.mean(tmp))
time_stdev1.append(statistics.stdev(tmp))

print("rectangular qubits:")
print(time_mean1)
print(time_stdev1)
