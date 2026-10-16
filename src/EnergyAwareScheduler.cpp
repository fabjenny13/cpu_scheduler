#include "EnergyAwareScheduler.h"
#include "Frequency.h"


FrequencyLevel EnergyAwareScheduler::chooseFrequency(
    const Process& process
) const
{
    // Medium has the same modeled energy per unit of work as low, but takes
    // half as long. Reserve high frequency for latency-sensitive processes.
    return process.priority <= 3
        ? FrequencyLevel::HIGH
        : FrequencyLevel::MEDIUM;
}


CPUCore* EnergyAwareScheduler::chooseCore(
    std::vector<CPUCore>& cores,
    const Process& process
) const
{
    CPUCore* bestCore = nullptr;
    double bestScore = 1e9;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        const bool needsFastCore = process.priority <= 3;
        double score = core.getUtilization() +
            ((core.isPerformanceCore() == needsFastCore) ? 0.0 : 25.0);
        if (score < bestScore)
        {
            bestScore = score;
            bestCore = &core;
        }
    }

    return bestCore;
}


void EnergyAwareScheduler::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    // Re-evaluate running jobs every tick so DVFS decisions respond to the
    // active workload instead of remaining fixed for an entire process.
    for (auto& core : cores)
    {
        if (!core.isBusy())
            continue;
        for (const auto& process : processes)
        {
            if (process.pid == core.getCurrentProcess())
            {
                core.setFrequency(getFrequencyGHz(chooseFrequency(process)));
                break;
            }
        }
    }

    for (auto& process : processes)
    {
        if (process.arrival_time > current_time ||
            process.blocked_until > current_time)
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

        CPUCore* core = chooseCore(cores, process);

        if (core == nullptr)
            break;

        FrequencyLevel frequency = chooseFrequency(process);

        core->setFrequency(
            getFrequencyGHz(frequency)
        );
        core->assignProcess(process.pid);
    }
}
