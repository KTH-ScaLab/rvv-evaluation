import sys
if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <n>")
        sys.exit(1)

qubit = int(sys.argv[1])
filename = f"ghz_q{qubit}"

with open(filename, "w") as f:
    f.write(f"{qubit}\n")
    f.write(f"0 h 0\n")
    for i in range(1, qubit):
        f.write(f"{i} cnot 0 {i}\n")

