#include "FCFS.h"
#include <algorithm>

void fcfs(std::vector<Process>& processes)
{
    std::sort(processes.begin(), processes.end(),
        [](const Process& a, const Process& b)
        {
            return a.arrival_time < b.arrival_time;
        });

    int current_time = 0;

    for (auto& p : processes)
    {
        if (current_time < p.arrival_time)
            current_time = p.arrival_time;

        current_time += p.burst_time;

        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.response_time = p.waiting_time;
    }
}