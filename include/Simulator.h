#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>

#include "Process.h"
#include "CPUCore.h"
#include "Scheduler.h"

class Simulator
{
private:
    std::vector<Process> processes;
    std::vector<CPUCore> cores;
    Scheduler* scheduler;

    int current_time;

public:
    Simulator(
        const std::vector<Process>& processes,
        int number_of_cores,
        Scheduler* scheduler
    );

    double run();

    int getCurrentTime() const;
    const std::vector<Process>& getProcesses() const;
    void setBaselineFrequency(std::vector<CPUCore>& cores);
};

#endif