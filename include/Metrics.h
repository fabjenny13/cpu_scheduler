#ifndef METRICS_H
#define METRICS_H

#include <vector>
#include <string>
#include "Process.h"

void printResults(
    const std::vector<Process>& processes,
    const std::string& title
);

#endif