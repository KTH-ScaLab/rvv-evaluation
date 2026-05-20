import cirq
import numpy as np

#implement qv

def qv_circuit(qubits, seed=None):
    rng = np.random.default_rng(seed)
    n = len(qubits)
    circuit = cirq.Circuit()
    depth = n
    for d in range(depth):
        # Generate a random permutation of qubit indices
        perm = rng.permutation(n)
        # Apply random 2-qubit unitaries on pairs
        for i in range(0, n - 1, 2):
            q0 = qubits[perm[i]]
            q1 = qubits[perm[i + 1]]
            # Use random single-qubit rotations and a CNOT as an approximation
            circuit.append([
                cirq.rx(rng.uniform(0, 2*np.pi))(q0),
                cirq.ry(rng.uniform(0, 2*np.pi))(q1),
                cirq.CNOT(q0, q1)
            ])
        # Apply random single-qubit permutations (optional)
        rng.shuffle(qubits)

    return circuit

