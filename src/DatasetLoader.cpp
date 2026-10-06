#include "DatasetLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

WorkloadType DatasetLoader::parseWorkloadType(
    const std::string& type)
{
    if (type == "CPU_BOUND")
        return WorkloadType::CPU_BOUND;

    if (type == "IO_BOUND")
        return WorkloadType::IO_BOUND;

    if (type == "INTERACTIVE")
        return WorkloadType::INTERACTIVE;

    return WorkloadType::BACKGROUND;
}

std::vector<Process> DatasetLoader::loadCSV(
    const std::string& filename)
{
    std::vector<Process> processes;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open dataset: "
                  << filename << "\n";

        return processes;
    }

    std::string line;

    // Skip CSV header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string value;

        Process process;

        // PID
        std::getline(ss, value, ',');
        process.pid = std::stoi(value);

        // ArrivalTime
        std::getline(ss, value, ',');
        process.arrival_time = std::stoi(value);

        // BurstTime
        std::getline(ss, value, ',');
        process.burst_time = std::stoi(value);

        // Remaining time starts equal to burst time
        process.remaining_time = process.burst_time;

        // CPUUtilization
        std::getline(ss, value, ',');
        process.cpu_utilization = std::stod(value);

        // IOWait
        std::getline(ss, value, ',');
        process.io_wait = std::stod(value);

        // Priority
        std::getline(ss, value, ',');
        process.priority = std::stoi(value);

        // WorkloadType
        std::getline(ss, value, ',');
        process.workload_type = parseWorkloadType(value);

        // Initialize scheduling results
        process.start_time = -1;
        process.completion_time = -1;
        process.waiting_time = 0;
        process.turnaround_time = 0;
        process.response_time = -1;

        processes.push_back(process);
    }

    file.close();

    std::cout << "Loaded "
              << processes.size()
              << " processes from "
              << filename
              << "\n";

    return processes;
}