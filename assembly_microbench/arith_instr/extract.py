#!/usr/bin/env python3
import sys
import math
from statistics import mean, stdev

def extract_times(filename, words, part):
    times = []
    with open(filename, "r") as f:
        for line in f:
            if words in line:
                parts = line.split(";")
                print(parts)
                if len(parts) >= part+1:
                    times.append(float(parts[part]))
    return times

def reshape_list(lst, size):
    return [lst[i:i + size] for i in range(0, len(lst), size)]

def print_grid(grid):
    for row in grid:
        print(*row)

if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} <file> <repeatition> <cols>")
        sys.exit(1)

file = sys.argv[1]
repeatition = int(sys.argv[2])
col = int(sys.argv[3])

times = extract_times(file, "GOps/sec", 1)
times_list_2d = reshape_list(times, repeatition)
if repeatition== 1:
    average_times_list=[i[0] for i in times_list_2d]
    stddev_times_list = [0 for i in times_list_2d]
else:
    average_times_list = [mean(i) for i in times_list_2d]
    stddev_times_list = [stdev(i) for i in times_list_2d]
    stddev_times_list = [ a/b*100 for a, b in zip(stddev_times_list,average_times_list)]

instr = extract_times(file, "instructions", 0)
instr_list_2d = reshape_list(instr, repeatition)
if repeatition== 1:
    average_instr_list = [i[0] for i in instr_list_2d]
    stddev_instr_list = [0 for i in instr_list_2d]
else:
    average_instr_list = [mean(i) for i in instr_list_2d]
    stddev_instr_list = [stdev(i) for i in instr_list_2d]
    stddev_instr_list = [ a/b*100 for a, b in zip(stddev_instr_list,average_instr_list)]
print("------------")
print("\nSimulation Time(s)\n")
print_grid(reshape_list(average_times_list,col))
print("\nStdev\n")
print_grid(reshape_list(stddev_times_list,col))
print("------------")
print("\nInstructions\n")
print_grid(reshape_list(average_instr_list,col))
print("\nStdev\n")
print_grid(reshape_list(stddev_instr_list,col))
