#include "Metrics.h"

#include <iostream>
#include <iomanip>

void printResults(
    const std::vector<Process>& processes,
    const std::string& title)
{
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    std::cout << "\n===== " << title << " =====\n";

    std::cout
        << std::left
        << std::setw(6) << "PID"
        << std::setw(10) << "Arrival"
        << std::setw(10) << "Burst"
        << std::setw(12) << "Completion"
        << std::setw(10) << "Waiting"
        << std::setw(12) << "Turnaround"
        << std::setw(10) << "Response"
        << "\n";

    for (const auto& p : processes)
    {
        std::cout
            << std::setw(6) << p.pid
            << std::setw(10) << p.arrival_time
            << std::setw(10) << p.burst_time
            << std::setw(12) << p.completion_time
            << std::setw(10) << p.waiting_time
            << std::setw(12) << p.turnaround_time
            << std::setw(10) << p.response_time
            << "\n";

        total_waiting += p.waiting_time;
        total_turnaround += p.turnaround_time;
        total_response += p.response_time;
    }

    std::cout << std::fixed << std::setprecision(2);

    std::cout
        << "Average Waiting Time: "
        << total_waiting / processes.size()
        << "\n";

    std::cout
        << "Average Turnaround Time: "
        << total_turnaround / processes.size()
        << "\n";

    std::cout
        << "Average Response Time: "
        << total_response / processes.size()
        << "\n";
}