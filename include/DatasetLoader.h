#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H

#include <vector>
#include <string>
#include "Process.h"

class DatasetLoader
{
public:
    static std::vector<Process> loadCSV(
        const std::string& filename
    );

private:
    static WorkloadType parseWorkloadType(
        const std::string& type
    );
};

#endif