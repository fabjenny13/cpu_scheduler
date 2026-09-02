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


void printResults(
    const std::vector<Process>& processes,
    const std::string& schedulerName)
{
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    std::cout << "\n===== "
              << schedulerName
              << " =====\n";

    std::cout << std::left
              << std::setw(6) << "PID"
              << std::setw(10) << "Arrival"
              << std::setw(10) << "Burst"
              << std::setw(12) << "Completion"
              << std::setw(10) << "Waiting"
              << std::setw(12) << "Turnaround"
              << std::setw(10) << "Response"
              << '\n';

    for (const auto& process : processes)
    {
        std::cout << std::setw(6) << process.pid
                  << std::setw(10) << process.arrival_time
                  << std::setw(10) << process.burst_time
                  << std::setw(12) << process.completion_time
                  << std::setw(10) << process.waiting_time
                  << std::setw(12) << process.turnaround_time
                  << std::setw(10) << process.response_time
                  << '\n';

        totalWaiting += process.waiting_time;
        totalTurnaround += process.turnaround_time;
        totalResponse += process.response_time;
    }

    int n = processes.size();

    if (n > 0)
    {
        std::cout << std::fixed
                  << std::setprecision(2);

        std::cout << "Average Waiting Time: "
                  << totalWaiting / n << '\n';

        std::cout << "Average Turnaround Time: "
                  << totalTurnaround / n << '\n';

        std::cout << "Average Response Time: "
                  << totalResponse / n << '\n';
    }
}