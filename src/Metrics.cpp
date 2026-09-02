#include "Metrics.h"

#include <iostream>
#include <iomanip>

Metrics calculateMetrics(
    const std::vector<Process>& processes,
    double totalEnergy)
{
    Metrics result{0.0, 0.0, 0.0, totalEnergy};

    if (processes.empty())
        return result;

    for (const auto& process : processes)
    {
        result.averageWaitingTime += process.waiting_time;
        result.averageTurnaroundTime += process.turnaround_time;
        result.averageResponseTime += process.response_time;
    }

    result.averageWaitingTime /= processes.size();
    result.averageTurnaroundTime /= processes.size();
    result.averageResponseTime /= processes.size();

    return result;
}

void printMetrics(
    const std::vector<Process>& processes,
    double totalEnergy)
{
    Metrics metrics =
        calculateMetrics(processes, totalEnergy);

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Average Waiting Time: "
              << metrics.averageWaitingTime << '\n';

    std::cout << "Average Turnaround Time: "
              << metrics.averageTurnaroundTime << '\n';

    std::cout << "Average Response Time: "
              << metrics.averageResponseTime << '\n';

    std::cout << "Total Energy: "
              << metrics.totalEnergy << " J\n";
}