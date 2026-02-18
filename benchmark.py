#!/usr/bin/env python3
import subprocess
import re

commands = [
    "./build/sorting_on_CPU/cpu_time < tests/data/1.txt",
    "./build/sorting_on_GPU/gpu_time < tests/data/1.txt",
]

iterations = 200

def get_time_from_command(command):
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    match = re.search(r'Total time:\s*([\d.]+)\s*ms', result.stdout)
    return float(match.group(1)) if match else None

print("Command 1: cpu")
print("Command 2: gpu")

print("Start benchmark...\n")

for i, cmd in enumerate(commands[:2], 1):
    times = []
    for _ in range(iterations):
        t = get_time_from_command(cmd)
        if t:
            times.append(t)

    if times:
        avg = sum(times) / len(times)
        print(f"Command {i}: Average time = {avg:.5f} ms (from {len(times)} measurments)")
    else:
        print(f"Command {i}: ❌ Error")
