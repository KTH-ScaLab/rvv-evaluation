import cirq
import numpy as np

#implement ghz
def ghz_circuit(qubits):
    circuit = cirq.Circuit()
    circuit.append(cirq.H(qubits[0]))
    for i in range(1, len(qubits)):
        circuit.append(cirq.CNOT(qubits[0], qubits[i]))
    return circuit

