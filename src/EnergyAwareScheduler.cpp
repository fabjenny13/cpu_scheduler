#include "EnergyAwareScheduler.h"
#include "Frequency.h"

FrequencyLevel EnergyAwareScheduler::chooseFrequency(
    const Process& process) const
{
    switch (process.workload)
    {
        case WorkloadType::CPU_BOUND:
            return FrequencyLevel::HIGH;

        case WorkloadType::INTERACTIVE:
            return FrequencyLevel::MEDIUM;

        case WorkloadType::IO_BOUND:
            return FrequencyLevel::LOW;

        case WorkloadType::BACKGROUND:
            return FrequencyLevel::LOW;
    }

    return FrequencyLevel::MEDIUM;
}

CPUCore* EnergyAwareScheduler::chooseCore(
    const Process& process,
    std::vector<CPUCore>& cores
) const
{
    CPUCore* fallback = nullptr;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        // Prefer performance cores for CPU-heavy/interactive work.
        if ((process.workload == WorkloadType::CPU_BOUND ||
             process.workload == WorkloadType::INTERACTIVE)
            && core.isPerformanceCore())
        {
            return &core;
        }

        // Keep an available core as fallback.
        if (fallback == nullptr)
            fallback = &core;
    }

    return fallback;
}

void EnergyAwareScheduler::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    for (auto& process : processes)
    {
        // Ignore processes that haven't arrived.
        if (process.arrival_time > current_time)
            continue;

        // Ignore completed processes.
        if (process.remaining_time <= 0)
            continue;

        // Don't schedule a process that is already running.
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

        CPUCore* core = chooseCore(process, cores);

        if (core == nullptr)
            continue;

        FrequencyLevel frequency =
            chooseFrequency(process);

        core->setFrequency(
            getFrequencyGHz(frequency)
        );

        core->assignProcess(process.pid);

        // For now we stop here.
        // Simulator will actually execute the process.
    }
}