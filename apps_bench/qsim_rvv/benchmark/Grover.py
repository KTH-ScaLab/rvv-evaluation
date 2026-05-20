import cirq
import numpy as np

#implement Grover
def grover_circuit(qubits, oracle_qubit=1):
    n = len(qubits)
    circuit = cirq.Circuit()
    
    # Initialization
    circuit.append(cirq.H.on_each(*qubits))
    
    # Oracle (mark the target state)
    circuit.append(cirq.Z(qubits[oracle_qubit]))
    
    # Diffusion operator
    circuit.append(cirq.H.on_each(*qubits))
    circuit.append(cirq.X.on_each(*qubits))
    circuit.append(cirq.H(qubits[-1]))
    circuit.append(cirq.CNOT(qubits[0], qubits[-1]))
    circuit.append(cirq.H(qubits[-1]))
    circuit.append(cirq.X.on_each(*qubits))
    circuit.append(cirq.H.on_each(*qubits))
    
    return circuit

