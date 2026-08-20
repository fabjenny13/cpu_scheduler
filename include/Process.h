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
    int remaining_time;   // used for Round Robin
    int completion_time;
    int waiting_time;
    int turnaround_time;

    //to be added soon:
    //WorkloadType workload_type;
};