#include <iostream>
#include <vector>

#include "Process.h"
#include "FCFS.h"
#include "SJF.h"
#include "RoundRobin.h"
#include "EnergyAwareScheduler.h"
#include "Simulator.h"
#include "Metrics.h"

int main()
{
    std::vector<Process> processes =
    {
        {1, 0, 5, 5, -1, -1, 0, 0, 0, WorkloadType::CPU_BOUND},
        {2, 1, 3, 3, -1, -1, 0, 0, 0, WorkloadType::IO_BOUND},
        {3, 2, 8, 8, -1, -1, 0, 0, 0, WorkloadType::BACKGROUND},
        {4, 3, 6, 6, -1, -1, 0, 0, 0, WorkloadType::INTERACTIVE}
    };

    auto fcfs_processes = processes;
    auto sjf_processes = processes;
    auto rr_processes = processes;

    fcfs(fcfs_processes);
    sjf(sjf_processes);
    roundRobin(rr_processes, 2);

    printResults(fcfs_processes, "FCFS");
    printResults(sjf_processes, "SJF");
    printResults(rr_processes, "Round Robin");

    EnergyAwareScheduler scheduler;

    Simulator simulator(
        processes,
        2,
        &scheduler
    );

    simulator.run();

    printResults(
        simulator.getProcesses(),
        "Energy Aware Scheduler"
    );

    return 0;
}