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
    // Remaining CPU work. DVFS can complete fractional work per time tick.
    double remaining_time;
    double work_completed = 0.0;
    // Elapsed time spent executing on a CPU (excludes I/O blocking).
    double service_time;

    // Scheduling results
    int start_time;
    int completion_time;
    double waiting_time;
    int turnaround_time;
    int response_time;

    // Workload characteristics
    double cpu_utilization;
    double io_wait;
    int priority;

    WorkloadType workload_type;

    // Simulator state for I/O-capable workloads. IOWait is interpreted as a
    // percentage of elapsed time spent waiting for I/O.
    int blocked_until = 0;
    double next_io_at = 0.0;
    double io_work_interval = 0.0;
    int io_wait_per_event = 0;
    int io_events_remaining = 0;
    double io_time = 0.0;
};

#endif
