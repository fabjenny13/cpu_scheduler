#ifndef ENERGY_AWARE_SCHEDULER_H
#define ENERGY_AWARE_SCHEDULER_H

#include "Scheduler.h"
#include "SchedulingDecision.h"

class EnergyAwareScheduler : public Scheduler
{
private:
    FrequencyLevel chooseFrequency(
        const CPUCore& core
    ) const;

    CPUCore* chooseCore(
        std::vector<CPUCore>& cores
    ) const;

public:
    void schedule(
        std::vector<Process>& processes,
        std::vector<CPUCore>& cores,
        int current_time
    ) override;
};

#endif