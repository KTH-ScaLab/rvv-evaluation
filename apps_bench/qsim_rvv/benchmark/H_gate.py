import cirq
import qsimcirq
import numpy as np
import time

num = [20, 24, 28]
for n in num:
    # Create circuit
    circuit = cirq.Circuit()
    circuit.append(cirq.H(q) for q in cirq.LineQubit.range(n))
    # print(circuit)

    # Simulate using Cirq's built-in simulator
    cirq_simulator = cirq.Simulator()
    cirq_result = cirq_simulator.simulate(circuit)

    # Simulate using qsim
    options = {'t': 4}
    qsim_simulator = qsimcirq.QSimSimulator(options)
    qsim_start = time.time()
    qsim_result = qsim_simulator.simulate(circuit)
    print(time.time()-qsim_start)
    # Check closeness
    if np.allclose(cirq_result.final_state_vector, qsim_result.final_state_vector, atol=1e-6):
        print("\n The state vectors match!")
    else:
        print("\n The state vectors do not match.")
