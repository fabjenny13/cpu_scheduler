#include <iostream>
#include <vector>

#include "Process.h"
#include "WorkloadEnergyAwareScheduler.h"
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

    WorkloadEnergyAwareScheduler scheduler;

    Simulator simulator(processes, 2, &scheduler);
    simulator.run();

    printResults(simulator.getProcesses(), "Workload Energy Aware Scheduler");

    return 0;
}
