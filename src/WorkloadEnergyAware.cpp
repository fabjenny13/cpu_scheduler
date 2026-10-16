#include "WorkloadEnergyAware.h"
#include "Frequency.h"

#include <algorithm>

FrequencyLevel WorkloadEnergyAwareScheduler::chooseFrequency(
    const CPUCore& core,
    const Process& process
) const
{
    // Interactive workloads need quick response
    if (process.workload_type == WorkloadType::INTERACTIVE)
        return FrequencyLevel::HIGH;

    // Low and medium have equal modeled energy per unit of CPU work;
    // medium finishes twice as quickly, so use it for non-urgent work.
    if (process.workload_type == WorkloadType::BACKGROUND ||
        process.workload_type == WorkloadType::IO_BOUND)
        return FrequencyLevel::MEDIUM;

    // CPU-bound workloads benefit from higher frequency
    if (process.workload_type == WorkloadType::CPU_BOUND)
    {
        if (process.cpu_utilization > 70.0)
            return FrequencyLevel::HIGH;

        return FrequencyLevel::MEDIUM;
    }

    // Fallback: use observed core utilization
    double utilization = core.getUtilization();

    if (utilization <= 30.0)
        return FrequencyLevel::LOW;

    if (utilization <= 70.0)
        return FrequencyLevel::MEDIUM;

    return FrequencyLevel::HIGH;
}

CPUCore* WorkloadEnergyAwareScheduler::chooseCore(
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

        double score = 0.0;

        // CPU-bound workloads benefit from performance cores
        if (process.workload_type == WorkloadType::CPU_BOUND)
        {
            if (core.isPerformanceCore())
                score -= 30.0;
            else
                score += 10.0;
        }

        // I/O-bound workloads are better suited to efficiency cores
        if (process.workload_type == WorkloadType::IO_BOUND)
        {
            if (!core.isPerformanceCore())
                score -= 30.0;
            else
                score += 10.0;
        }

        // Interactive processes prioritize responsiveness
        if (process.workload_type == WorkloadType::INTERACTIVE)
        {
            if (core.isPerformanceCore())
                score -= 40.0;
            else
                score += 10.0;
        }

        // Background processes prioritize energy efficiency
        if (process.workload_type == WorkloadType::BACKGROUND)
        {
            if (!core.isPerformanceCore())
                score -= 40.0;
            else
                score += 10.0;
        }

        // High CPU utilization favors performance cores
        if (process.cpu_utilization > 70.0)
        {
            if (core.isPerformanceCore())
                score -= 20.0;
        }

        // High I/O wait favors efficiency cores
        if (process.io_wait > 60.0)
        {
            if (!core.isPerformanceCore())
                score -= 20.0;
        }

        // Higher priority means responsiveness is more important
        if (process.priority <= 3 &&
            core.isPerformanceCore())
        {
            score -= 15.0;
        }

        // Prefer the currently less-utilized core
        score += core.getUtilization();

        if (score < bestScore)
        {
            bestScore = score;
            bestCore = &core;
        }
    }

    return bestCore;
}

void WorkloadEnergyAwareScheduler::schedule(
    std::vector<Process>& processes,
    std::vector<CPUCore>& cores,
    int current_time)
{
    // Keep the chosen DVFS level current as process state and core load change.
    for (auto& core : cores)
    {
        if (!core.isBusy())
            continue;
        for (const auto& process : processes)
        {
            if (process.pid == core.getCurrentProcess())
            {
                core.setFrequency(getFrequencyGHz(chooseFrequency(core, process)));
                break;
            }
        }
    }

    std::vector<Process*> ready;
    for (auto& process : processes)
    {
        if (process.arrival_time <= current_time &&
            process.blocked_until <= current_time && process.remaining_time > 0)
            ready.push_back(&process);
    }
    std::sort(ready.begin(), ready.end(), [](const Process* a, const Process* b)
    {
        if (a->arrival_time != b->arrival_time)
            return a->arrival_time < b->arrival_time;
        if (a->priority != b->priority)
            return a->priority < b->priority;
        return a->pid < b->pid;
    });

    for (Process* process_ptr : ready)
    {
        Process& process = *process_ptr;

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

        FrequencyLevel frequency =
            chooseFrequency(*core, process);

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
