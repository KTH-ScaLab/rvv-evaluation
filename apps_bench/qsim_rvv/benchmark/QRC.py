import cirq
import numpy as np

#implement random
def random_circuit(qubits, depth=64):
    circuit = cirq.Circuit()
    n = len(qubits)
    for _ in range(depth):
        for q in qubits:
            circuit.append(cirq.rx(np.random.uniform(0, 2*np.pi))(q))
            circuit.append(cirq.ry(np.random.uniform(0, 2*np.pi))(q))
            circuit.append(cirq.rz(np.random.uniform(0, 2*np.pi))(q))
        # Add some random two-qubit gates
        for i in range(0, n - 1, 2):
            circuit.append(cirq.CZ(qubits[i], qubits[i+1]))
    return circuit
