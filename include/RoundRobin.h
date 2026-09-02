#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "Scheduler.h"

class RoundRobin : public Scheduler
{
private:
    int quantum;

public:
    RoundRobin(int quantum);

    void schedule(
        std::vector<Process>& processes,
        std::vector<CPUCore>& cores,
        int current_time
    ) override;
};

#endif