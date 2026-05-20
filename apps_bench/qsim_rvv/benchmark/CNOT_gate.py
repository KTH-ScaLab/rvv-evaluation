import cirq
import qsimcirq
import numpy as np
import time

# Define 2 qubits
qubits = cirq.LineQubit.range(4)

# Create the circuit
circuit = cirq.Circuit()
circuit.append(cirq.H(qubits[0]))               # Put qubit 0 into superposition
circuit.append(cirq.CNOT(qubits[0], qubits[2])) # Entangle qubit 1 with qubit 0

# Simulate with Cirq's simulator
cirq_simulator = cirq.Simulator()
cirq_result = cirq_simulator.simulate(circuit)

# Simulate with your custom qsim simulator
options = {'t': 1}  # single thread is enough for 2 qubits
qsim_simulator = qsimcirq.QSimSimulator(options)
qsim_start = time.time()
qsim_result = qsim_simulator.simulate(circuit)
print(f"qsim simulation time: {time.time() - qsim_start:.6f} seconds")

# Compare final state vectors
if np.allclose(cirq_result.final_state_vector, qsim_result.final_state_vector, atol=1e-6):
    print("The state vectors match. CNOT gate seems correct.")
else:
    print("The state vectors do NOT match. CNOT gate may be incorrect.")
    # Print both vectors for debugging
    print("Cirq result:", cirq_result.final_state_vector)
    print("qsim result:", qsim_result.final_state_vector)

