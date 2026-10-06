#ifndef PROCESS_H
#define PROCESS_H

#include <string>

enum class WorkloadType
{
    CPU_BOUND,
    IO_BOUND,
    BACKGROUND,
    INTERACTIVE
};

struct Process
{
    // Scheduling information
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;

    // Scheduling results
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;

    // Workload characteristics
    double cpu_utilization;
    double io_wait;
    int priority;

    WorkloadType workload;
};

#endif