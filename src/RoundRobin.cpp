#include "RoundRobin.h"
#include <algorithm>
#include <queue>

void roundRobin(std::vector<Process>& processes, int quantum)
{
    int n = processes.size();

    for (auto& p : processes)
        p.remaining_time = p.burst_time;

    std::sort(processes.begin(), processes.end(),
        [](const Process& a, const Process& b)
        {
            return a.arrival_time < b.arrival_time;
        });

    std::queue<int> ready_queue;

    int current_time = 0;
    int completed = 0;
    int next_process = 0;

    while (completed < n)
    {
        // Add processes that have arrived.
        while (next_process < n &&
               processes[next_process].arrival_time <= current_time)
        {
            ready_queue.push(next_process);
            next_process++;
        }

        // If nothing is ready, jump to the next arrival.
        if (ready_queue.empty())
        {
            current_time = processes[next_process].arrival_time;
            continue;
        }

        int index = ready_queue.front();
        ready_queue.pop();

        Process& p = processes[index];

        // First time this process gets CPU.
        if (p.start_time == -1)
        {
            p.start_time = current_time;
            p.response_time =
                p.start_time - p.arrival_time;
        }

        int execution_time =
            std::min(quantum, p.remaining_time);

        p.remaining_time -= execution_time;
        current_time += execution_time;

        // Add processes that arrived during execution.
        while (next_process < n &&
               processes[next_process].arrival_time <= current_time)
        {
            ready_queue.push(next_process);
            next_process++;
        }

        if (p.remaining_time > 0)
        {
            ready_queue.push(index);
        }
        else
        {
            p.completion_time = current_time;

            p.turnaround_time =
                p.completion_time - p.arrival_time;

            p.waiting_time =
                p.turnaround_time - p.burst_time;

            completed++;
        }
    }
}