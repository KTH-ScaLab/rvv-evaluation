import cirq
import numpy as np

#implement QFT
def qft_circuit(qubits):
    circuit = cirq.Circuit()
    n = len(qubits)

    for i in range(n):
        circuit.append(cirq.H(qubits[i]))
        for j in range(i+1, n):
            angle = np.pi / 2**(j - i)
            circuit.append(cirq.CZ(qubits[j], qubits[i])**(angle / np.pi))

    # Swap qubits to reverse the order
    for i in range(n // 2):
        circuit.append(cirq.SWAP(qubits[i], qubits[n - i - 1]))

    return circuit
