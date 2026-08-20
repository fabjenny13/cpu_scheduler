#include "SJF.h"
#include <climits>

void sjf(std::vector<Process>& processes)
{
    int n = processes.size();

    std::vector<bool> completed(n, false);

    int completed_count = 0;
    int current_time = 0;

    while (completed_count < n)
    {
        int selected = -1;
        int shortest_burst = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].burst_time < shortest_burst)
            {
                shortest_burst = processes[i].burst_time;
                selected = i;
            }
        }

        // No process has arrived yet.
        if (selected == -1)
        {
            int next_arrival = INT_MAX;

            for (int i = 0; i < n; i++)
            {
                if (!completed[i])
                    next_arrival =
                        std::min(next_arrival, processes[i].arrival_time);
            }

            current_time = next_arrival;
            continue;
        }

        // Execute selected process.
        if (processes[selected].start_time == -1)
        {
            processes[selected].start_time = current_time;
        }

        current_time += processes[selected].burst_time;

        processes[selected].completion_time = current_time;

        processes[selected].turnaround_time =
            processes[selected].completion_time -
            processes[selected].arrival_time;

        processes[selected].waiting_time =
            processes[selected].turnaround_time -
            processes[selected].burst_time;

        processes[selected].response_time =
            processes[selected].start_time -
            processes[selected].arrival_time;

        completed[selected] = true;
        completed_count++;
    }
}