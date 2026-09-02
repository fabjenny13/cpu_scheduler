#include "RoundRobin.h"
#include <queue>

RoundRobin::RoundRobin(int quantum)
{
    this->quantum = quantum;
}

void RoundRobin::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    // Find processes that are ready and not currently running.
    std::vector<int> ready;

    for (int i = 0; i < processes.size(); i++)
    {
        if (processes[i].arrival_time <= current_time &&
            processes[i].remaining_time > 0)
        {
            bool already_running = false;

            for (const auto& core : cores)
            {
                if (core.isBusy() &&
                    core.getCurrentProcess() == processes[i].pid)
                {
                    already_running = true;
                    break;
                }
            }

            if (!already_running)
                ready.push_back(i);
        }
    }

    // Assign ready processes to free cores.
    int next = 0;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        if (next >= ready.size())
            break;

        int index = ready[next];

        core.assignProcess(processes[index].pid);

        next++;
    }
}