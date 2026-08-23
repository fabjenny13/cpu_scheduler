#include "EnergyAwareScheduler.h"
#include "Frequency.h"

FrequencyLevel EnergyAwareScheduler::chooseFrequency(
    const std::vector<CPUCore>& cores
) const
{
    int busy_cores = 0;

    for (const auto& core : cores)
    {
        if (core.isBusy())
            busy_cores++;
    }

    double utilization =
        static_cast<double>(busy_cores) / cores.size();

    if (utilization >= 0.75)
        return FrequencyLevel::HIGH;

    if (utilization >= 0.50)
        return FrequencyLevel::MEDIUM;

    return FrequencyLevel::LOW;
}

CPUCore* EnergyAwareScheduler::chooseCore(
    std::vector<CPUCore>& cores
) const
{
    CPUCore* efficiency_core = nullptr;
    CPUCore* performance_core = nullptr;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        if (core.isPerformanceCore())
        {
            if (performance_core == nullptr)
                performance_core = &core;
        }
        else
        {
            if (efficiency_core == nullptr)
                efficiency_core = &core;
        }
    }

    // Prefer efficiency cores to save energy.
    if (efficiency_core != nullptr)
        return efficiency_core;

    return performance_core;
}


void EnergyAwareScheduler::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    FrequencyLevel frequency =
        chooseFrequency(cores);

    double frequencyGHz =
        getFrequencyGHz(frequency);

    for (auto& process : processes)
    {
        if (process.arrival_time > current_time)
            continue;

        if (process.remaining_time <= 0)
            continue;

        // Don't schedule a process already running.
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

        CPUCore* core = chooseCore(cores);

        if (core == nullptr)
            break;

        core->setFrequency(frequencyGHz);
        core->assignProcess(process.pid);
    }
}