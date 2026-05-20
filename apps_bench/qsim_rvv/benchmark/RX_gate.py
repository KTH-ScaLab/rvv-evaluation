import cirq
import qsimcirq
import numpy as np

n = 20
angle = np.pi / 4
qubits = cirq.LineQubit.range(n)

circuit = cirq.Circuit()
circuit.append(cirq.rx(angle)(q) for q in qubits)

# Cirq simulation
cirq_sim = cirq.Simulator()
cirq_result = cirq_sim.simulate(circuit)

# qsim simulation
qsim_sim = qsimcirq.QSimSimulator()
qsim_result = qsim_sim.simulate(circuit)

print("RX circuit for", n, "qubits:")
print(circuit)

# Print results
print("Cirq final state vector:\n", cirq_result.final_state_vector)
print("\nQSim final state vector:\n", qsim_result.final_state_vector)
print("\nMatch:", np.allclose(cirq_result.final_state_vector, qsim_result.final_state_vector, atol=1e-6))

