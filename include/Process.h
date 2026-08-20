#ifndef PROCESS_H
#define PROCESS_H

enum class WorkloadType {
    CPU_BOUND,
    IO_BOUND,
    INTERACTIVE,
    BACKGROUND
};

struct Process {
    int pid;

    int arrival_time;
    int burst_time;
    int remaining_time;

    int start_time = -1;
    int completion_time = -1;

    int waiting_time = 0;
    int turnaround_time = 0;
    int response_time = 0;

    WorkloadType workload;
};

#endif