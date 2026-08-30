#include "WorkloadEnergyAwareScheduler.h"
#include "Frequency.h"
#include "PowerModel.h"

#include <iostream>

FrequencyLevel WorkloadEnergyAwareScheduler::chooseFrequency(
    const CPUCore& core,
    WorkloadType workload
) const
{
    if (workload == WorkloadType::INTERACTIVE)
        return FrequencyLevel::HIGH;

    if (workload == WorkloadType::BACKGROUND)
        return FrequencyLevel::LOW;

    double utilization = core.getUtilization();

    if (workload == WorkloadType::IO_BOUND)
        return utilization <= 50.0
            ? FrequencyLevel::LOW
            : FrequencyLevel::MEDIUM;

    if (utilization <= 30.0)
        return FrequencyLevel::LOW;

    if (utilization <= 70.0)
        return FrequencyLevel::MEDIUM;

    return FrequencyLevel::HIGH;
}


CPUCore* WorkloadEnergyAwareScheduler::chooseCore(
    std::vector<CPUCore>& cores,
    WorkloadType workload
) const
{
    bool wants_performance_core =
        (workload == WorkloadType::INTERACTIVE ||
         workload == WorkloadType::CPU_BOUND);

    CPUCore* bestPreferred = nullptr;
    CPUCore* bestAny = nullptr;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        if (bestAny == nullptr ||
            core.getUtilization() < bestAny->getUtilization())
        {
            bestAny = &core;
        }

        if (core.isPerformanceCore() != wants_performance_core)
            continue;

        if (bestPreferred == nullptr ||
            core.getUtilization() < bestPreferred->getUtilization())
        {
            bestPreferred = &core;
        }
    }

    return bestPreferred != nullptr ? bestPreferred : bestAny;
}


void WorkloadEnergyAwareScheduler::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    for (auto& process : processes)
    {
        if (process.arrival_time > current_time)
            continue;

        if (process.remaining_time <= 0)
            continue;

        bool already_running = false;

        for (auto& core : cores)
        {
            if (core.isBusy() &&
                core.getCurrentProcess() == process.pid)
            {
                already_running = true;
                break;
            }
        }

        if (already_running)
            continue;

        CPUCore* core = chooseCore(cores, process.workload);

        if (core == nullptr)
            break;

        FrequencyLevel frequency =
            chooseFrequency(*core, process.workload);

        double frequencyGHz = getFrequencyGHz(frequency);
        double power = PowerModel::getPower(frequencyGHz);

        std::cout << "Time " << current_time
          << ": P" << process.pid
          << " -> Core " << core->getId()
          << " (" << (core->isPerformanceCore() ? "perf" : "efficiency")
          << ") | Utilization: "
          << core->getUtilization()
          << "% | Frequency: "
          << getFrequencyGHz(frequency)
          << " GHz\n"
          << power
          << " W\n";

        core->setFrequency(getFrequencyGHz(frequency));
        core->assignProcess(process.pid);

        if (process.start_time == -1)
        {
            process.start_time = current_time;
            process.response_time =
                process.start_time - process.arrival_time;
        }
    }
}
