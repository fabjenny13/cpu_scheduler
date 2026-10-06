import csv
import random
import os

random.seed(42)

NUM_PROCESSES = 2000

workload_counts = {
    "CPU_BOUND": 600,
    "IO_BOUND": 600,
    "INTERACTIVE": 400,
    "BACKGROUND": 400
}

processes = []

pid = 1
arrival_time = 0

for workload, count in workload_counts.items():

    for _ in range(count):

        # Arrival time
        arrival_time += random.randint(0, 3)

        # Generate workload-specific characteristics
        if workload == "CPU_BOUND":
            burst_time = random.randint(15, 60)
            cpu_utilization = random.uniform(80, 100)
            io_wait = random.uniform(0, 20)
            priority = random.randint(4, 7)

        elif workload == "IO_BOUND":
            burst_time = random.randint(2, 12)
            cpu_utilization = random.uniform(10, 40)
            io_wait = random.uniform(60, 95)
            priority = random.randint(4, 7)

        elif workload == "INTERACTIVE":
            burst_time = random.randint(1, 8)
            cpu_utilization = random.uniform(40, 80)
            io_wait = random.uniform(20, 60)
            priority = random.randint(1, 3)

        else:  # BACKGROUND
            burst_time = random.randint(20, 80)
            cpu_utilization = random.uniform(5, 30)
            io_wait = random.uniform(40, 90)
            priority = random.randint(8, 10)

        processes.append([
            pid,
            arrival_time,
            burst_time,
            round(cpu_utilization, 2),
            round(io_wait, 2),
            priority,
            workload
        ])

        pid += 1


# Shuffle the workload types so they aren't grouped together
random.shuffle(processes)

# Reassign PID after shuffling
for i, process in enumerate(processes):
    process[0] = i + 1


# Create data directory
os.makedirs("../data", exist_ok=True)

output_file = "../data/synthetic_2000.csv"

with open(output_file, "w", newline="") as file:

    writer = csv.writer(file)

    writer.writerow([
        "PID",
        "ArrivalTime",
        "BurstTime",
        "CPUUtilization",
        "IOWait",
        "Priority",
        "WorkloadType"
    ])

    writer.writerows(processes)


print(f"Generated {NUM_PROCESSES} processes.")
print(f"Dataset saved to: {output_file}")