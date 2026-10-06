#ifndef WORKLOAD_ENERGY_AWARE_SCHEDULER_H
#define WORKLOAD_ENERGY_AWARE_SCHEDULER_H

#include "Scheduler.h"
#include "Frequency.h"

class WorkloadEnergyAwareScheduler : public Scheduler
{
private:
    FrequencyLevel chooseFrequency(
        const CPUCore& core,
        const Process& process
    ) const;

    CPUCore* chooseCore(
        std::vector<CPUCore>& cores,
        const Process& process
    ) const;

public:
bool controlsFrequency() const override
{
    return true;
}

    void schedule(
        std::vector<Process>& processes,
        std::vector<CPUCore>& cores,
        int current_time
    ) override;
};

#endif