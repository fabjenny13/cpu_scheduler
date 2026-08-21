#ifndef ENERGY_AWARE_SCHEDULER_H
#define ENERGY_AWARE_SCHEDULER_H

#include "Scheduler.h"
#include "CPUCore.h"
#include "SchedulingDecision.h"

class EnergyAwareScheduler : public Scheduler
{
private:
    FrequencyLevel chooseFrequency(
        const Process& process
    ) const;

    CPUCore* chooseCore(
        Process& process,
        std::vector<CPUCore>& cores
    ) const;

public:
    void schedule(
        std::vector<Process>& processes
    ) override;
};

#endif