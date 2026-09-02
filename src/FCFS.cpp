#include "FCFS.h"

void FCFS::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        int selected = -1;

        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrival_time > current_time)
                continue;

            if (processes[i].remaining_time <= 0)
                continue;

            // Check whether this process is already running.
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

            selected = i;
            break;
        }

        if (selected == -1)
            continue;

        core.assignProcess(processes[selected].pid);
    }
}