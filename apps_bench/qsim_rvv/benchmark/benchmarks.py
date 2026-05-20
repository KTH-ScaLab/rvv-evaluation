# benchmarks/__init__.py
from ghz import ghz_circuit
from Grover import grover_circuit
from QRC import random_circuit
from qft import qft_circuit
from qv import qv_circuit
BENCHMARKS = {
    "ghz": ghz_circuit,
    "grover": grover_circuit,
    "QRC" : random_circuit,
    "qft": qft_circuit,
    "qv": qv_circuit
}
