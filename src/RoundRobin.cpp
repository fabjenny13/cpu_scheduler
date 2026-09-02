#include "RoundRobin.h"

RoundRobin::RoundRobin(int quantum)
{
    this->quantum = quantum;
}

void RoundRobin::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    // Add newly arrived processes to the ready queue
    for (auto& process : processes)
    {
        if (process.arrival_time <= current_time &&
            process.remaining_time > 0 &&
            inQueue.find(process.pid) == inQueue.end())
        {
            bool alreadyRunning = false;

            for (const auto& core : cores)
            {
                if (core.isBusy() &&
                    core.getCurrentProcess() == process.pid)
                {
                    alreadyRunning = true;
                    break;
                }
            }

            if (!alreadyRunning)
            {
                readyQueue.push(process.pid);
                inQueue.insert(process.pid);
            }
        }
    }

    // First handle processes whose quantum has expired
    for (auto& core : cores)
    {
        if (!core.isBusy())
            continue;

        if (core.getTimeSlice() >= quantum)
        {
            int pid = core.getCurrentProcess();

            readyQueue.push(pid);
            inQueue.insert(pid);

            core.release();
        }
    }

    // Assign ready processes to free cores
    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        if (readyQueue.empty())
            break;

        int pid = readyQueue.front();
        readyQueue.pop();
        inQueue.erase(pid);

        core.assignProcess(pid);
    }
}