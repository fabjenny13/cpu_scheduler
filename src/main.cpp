#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

#include "Process.h"
#include "FCFS.h"
#include "SJF.h"
#include "RoundRobin.h"
#include "WorkloadEnergyAware.h"
#include "EnergyAwareScheduler.h"
#include "Simulator.h"
#include "Metrics.h"

namespace
{

void printComparisonRow(
    const std::string& name,
    const Metrics& m)
{
    std::cout << std::left << std::setw(24) << name
              << std::right
              << std::fixed << std::setprecision(2)
              << std::setw(12) << m.averageWaitingTime
              << std::setw(14) << m.averageTurnaroundTime
              << std::setw(12) << m.averageResponseTime
              << std::setw(14) << m.totalEnergy
              << '\n';
}

}

int main()
{
    std::vector<Process> processes =
    {
        {1, 0, 5, 5, -1, -1, 0, 0, 0, WorkloadType::CPU_BOUND},
        {2, 1, 3, 3, -1, -1, 0, 0, 0, WorkloadType::IO_BOUND},
        {3, 2, 8, 8, -1, -1, 0, 0, 0, WorkloadType::BACKGROUND},
        {4, 3, 6, 6, -1, -1, 0, 0, 0, WorkloadType::INTERACTIVE}
    };

    FCFS fcfs;
    Simulator fcfsSimulator(processes, 2, &fcfs);
    double fcfsEnergy = fcfsSimulator.run();
    Metrics fcfsMetrics = calculateMetrics(fcfsSimulator.getProcesses(), fcfsEnergy);
    printResults(fcfsSimulator.getProcesses(), "FCFS");

    SJF sjf;
    Simulator sjfSimulator(processes, 2, &sjf);
    double sjfEnergy = sjfSimulator.run();
    Metrics sjfMetrics = calculateMetrics(sjfSimulator.getProcesses(), sjfEnergy);
    printResults(sjfSimulator.getProcesses(), "SJF");

    RoundRobin rr(2);
    Simulator rrSimulator(processes, 2, &rr);
    double rrEnergy = rrSimulator.run();
    Metrics rrMetrics = calculateMetrics(rrSimulator.getProcesses(), rrEnergy);
    printResults(rrSimulator.getProcesses(), "Round Robin");

    EnergyAwareScheduler energyScheduler;
    Simulator energySimulator(processes, 2, &energyScheduler);
    double energyEnergy = energySimulator.run();
    Metrics energyMetrics = calculateMetrics(energySimulator.getProcesses(), energyEnergy);
    printResults(energySimulator.getProcesses(), "Energy Aware Scheduler");

    WorkloadEnergyAwareScheduler workloadScheduler;
    Simulator workloadSimulator(processes, 2, &workloadScheduler);
    double workloadEnergy = workloadSimulator.run();
    Metrics workloadMetrics = calculateMetrics(workloadSimulator.getProcesses(), workloadEnergy);
    printResults(workloadSimulator.getProcesses(), "Workload Energy Aware Scheduler");

    std::cout << "\n===== SCHEDULER COMPARISON =====\n";
    std::cout << std::left << std::setw(24) << "Scheduler"
              << std::right
              << std::setw(12) << "AvgWait"
              << std::setw(14) << "AvgTurnaround"
              << std::setw(12) << "AvgResponse"
              << std::setw(14) << "Energy(J)"
              << '\n';

    printComparisonRow("FCFS", fcfsMetrics);
    printComparisonRow("SJF", sjfMetrics);
    printComparisonRow("Round Robin", rrMetrics);
    printComparisonRow("Energy Aware", energyMetrics);
    printComparisonRow("Workload Energy Aware", workloadMetrics);

    double bestBaselineEnergy = std::min({fcfsMetrics.totalEnergy, sjfMetrics.totalEnergy, rrMetrics.totalEnergy, energyMetrics.totalEnergy});

    if (bestBaselineEnergy > 0.0)
    {
        double savingsPercent =
            100.0 * (bestBaselineEnergy - workloadMetrics.totalEnergy) / bestBaselineEnergy;

        std::cout << "\nWorkload-aware scheduler uses "
                  << std::fixed << std::setprecision(1)
                  << savingsPercent
                  << "% less energy than the best baseline scheduler.\n";
    }

    return 0;
}
