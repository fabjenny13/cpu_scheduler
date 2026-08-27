#include "EnergyAwareScheduler.h"
#include "Frequency.h"
#include "PowerModel.h"

#include <iostream>

FrequencyLevel EnergyAwareScheduler::chooseFrequency(
    const CPUCore& core
) const
{
    double utilization = core.getUtilization();

    if (utilization <= 30.0)
        return FrequencyLevel::LOW;

    if (utilization <= 70.0)
        return FrequencyLevel::MEDIUM;

    return FrequencyLevel::HIGH;
}


CPUCore* EnergyAwareScheduler::chooseCore(
    std::vector<CPUCore>& cores
) const
{
    CPUCore* bestCore = nullptr;

    for (auto& core : cores)
    {
        if (core.isBusy())
            continue;

        if (bestCore == nullptr)
        {
            bestCore = &core;
            continue;
        }

        // Choose the less utilized core.
        if (core.getUtilization() <
            bestCore->getUtilization())
        {
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

        FrequencyLevel frequency =
            chooseFrequency(*core);

        double frequencyGHz = getFrequencyGHz(frequency);

        double power = PowerModel::getPower(frequencyGHz);

        std::cout << "Time " << current_time
          << ": P" << process.pid
          << " -> Core " << core->getId()
          << " | Utilization: "
          << core->getUtilization()
          << "% | Frequency: "
          << getFrequencyGHz(frequency)
          << " GHz\n"
          << power
          << " W\n";

        core->setFrequency(
            getFrequencyGHz(frequency)
        );
        core->assignProcess(process.pid);
    }
}