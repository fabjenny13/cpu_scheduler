#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Process.h"

class Scheduler
{
public:
    virtual void schedule(std::vector<Process>& processes) = 0;

    virtual ~Scheduler() = default;
};

#endif