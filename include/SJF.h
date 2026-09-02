#ifndef SJF_H
#define SJF_H

#include "Scheduler.h"

class SJF : public Scheduler
{
public:
    void schedule(
        std::vector<Process>& processes,
        std::vector<CPUCore>& cores,
        int current_time
    ) override;
};

#endif