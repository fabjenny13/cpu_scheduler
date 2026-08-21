#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

#include "Process.h"
#include "CPUCore.h"

class Scheduler
{
public:
    virtual void schedule(
        std::vector<Process>& processes,
        std::vector<CPUCore>& cores,
        int current_time
    ) = 0;

    virtual ~Scheduler() = default;
};

#endif