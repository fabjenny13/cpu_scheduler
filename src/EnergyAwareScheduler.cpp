#include "EnergyAwareScheduler.h"

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
    Process& process,
    std::vector<CPUCore>& cores
) const
{
    CPUCore* best_core = nullptr;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        // CPU-bound and interactive workloads prefer
        // performance cores.
        if ((process.workload == WorkloadType::CPU_BOUND ||
             process.workload == WorkloadType::INTERACTIVE)
            && core.isPerformanceCore())
        {
            return &core;
        }

        // Background / I/O workloads can use any available core.
        if (best_core == nullptr)
        {
            best_core = &core;
        }
    }

    return best_core;
}

void EnergyAwareScheduler::schedule(
    std::vector<Process>& processes)
{
    // This is only the first version.
    //
    // The real scheduling loop will eventually
    // be controlled by Simulator.

    std::vector<CPUCore> cores;

    cores.emplace_back(0, true);
    cores.emplace_back(1, true);
    cores.emplace_back(2, false);
    cores.emplace_back(3, false);

    for (auto& process : processes)
    {
        CPUCore* core = chooseCore(process, cores);

        if (core == nullptr)
            continue;

        FrequencyLevel frequency =
            chooseFrequency(process);

        core->setFrequency(
            getFrequencyGHz(frequency)
        );

        core->assignProcess(process.pid);

        // For now, just calculate the first scheduling decision.
        // Execution and energy calculation will be handled
        // by Simulator later.

        core->release();
    }
}