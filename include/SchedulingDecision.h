#ifndef SCHEDULING_DECISION_H
#define SCHEDULING_DECISION_H

#include "Frequency.h"

struct SchedulingDecision
{
    int process_id;
    int core_id;
    FrequencyLevel frequency;
};

#endif