#ifndef ENERGY_AWARE_SCHEDULER_H
#define ENERGY_AWARE_SCHEDULER_H

#include "Scheduler.h"

class EnergyAwareScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process>& processes) override;
};

#endif