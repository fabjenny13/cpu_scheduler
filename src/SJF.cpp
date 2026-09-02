#include "SJF.h"
#include <climits>

void SJF::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    std::vector<bool> assigned(processes.size(), false);

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        int selected = -1;
        int shortest_burst = INT_MAX;

        for (int i = 0; i < processes.size(); i++)
        {
            if (assigned[i])
                continue;

            if (processes[i].arrival_time > current_time)
                continue;

            if (processes[i].remaining_time <= 0)
                continue;

            // Don't assign a process already running on another core.
            bool already_running = false;

            for (const auto& other_core : cores)
            {
                if (other_core.isBusy() &&
                    other_core.getCurrentProcess() == processes[i].pid)
                {
                    already_running = true;
                    break;
                }
            }

            if (already_running)
                continue;

            if (processes[i].burst_time < shortest_burst)
            {
                shortest_burst = processes[i].burst_time;
                selected = i;
            }
        }

        if (selected == -1)
            continue;

        core.assignProcess(processes[selected].pid);
        assigned[selected] = true;
    }
}