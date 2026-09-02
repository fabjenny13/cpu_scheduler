#ifndef METRICS_H
#define METRICS_H

#include <vector>
#include <string>

#include "Process.h"

struct Metrics
{
    double averageWaitingTime;
    double averageTurnaroundTime;
    double averageResponseTime;
    double totalEnergy;
};

Metrics calculateMetrics(
    const std::vector<Process>& processes,
    double totalEnergy
);


void printResults(
    const std::vector<Process>& processes,
    const std::string& schedulerName
);

#endif