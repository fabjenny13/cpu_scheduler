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


void printResults(
    const std::vector<Process>& processes,
    const std::string& schedulerName)
{
    double totalWaiting = 0.0;
    double totalTurnaround = 0.0;
    double totalResponse = 0.0;
    int completed = 0;

    std::cout << "\n===== "
              << schedulerName
              << " =====\n";

    for (const auto& process : processes)
    {
        if (process.completion_time < 0)
            continue;
        ++completed;
        totalWaiting += process.waiting_time;
        totalTurnaround += process.turnaround_time;
        totalResponse += process.response_time;
    }

    int n = completed;

    std::cout << "Completed processes: " << completed
              << " / " << processes.size() << '\n';

    if (n > 0)
    {
        std::cout << std::fixed
                  << std::setprecision(2);

        std::cout << "Average Waiting Time: "
                  << totalWaiting / n << '\n';

        std::cout << "Average Turnaround Time: "
                  << totalTurnaround / n << '\n';

        std::cout << "Average Response Time: "
                  << totalResponse / n << "\n\n";
    }

        std::cout << "==========================================\n";

}
