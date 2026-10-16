#include "Simulator.h"
#include "PowerModel.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Simulator::Simulator(
    const std::vector<Process>& processes,
    int number_of_cores,
    Scheduler* scheduler)
{
    this->processes = processes;

    for (auto& process : this->processes)
    {
        process.remaining_time = process.burst_time;
        process.work_completed = 0.0;
        process.service_time = 0.0;
        process.io_time = 0.0;
        process.blocked_until = 0;
        process.io_events_remaining = 0;
        process.next_io_at = 0.0;
        process.io_work_interval = 0.0;
        process.io_wait_per_event = 0;

        const bool can_block_for_io =
            process.workload_type != WorkloadType::CPU_BOUND;
        if (can_block_for_io && process.io_wait > 0.0 && process.burst_time > 1)
        {
            const double io_fraction =
                std::clamp(process.io_wait, 0.0, 95.0) / 100.0;
            process.io_events_remaining =
                std::max(1, process.burst_time / 4);
            process.io_work_interval = process.burst_time /
                static_cast<double>(process.io_events_remaining + 1);
            process.next_io_at = process.io_work_interval;
            const double total_io_time = process.burst_time * io_fraction /
                (1.0 - io_fraction);
            process.io_wait_per_event = std::max(1, static_cast<int>(std::ceil(
                total_io_time / process.io_events_remaining)));
        }
    }
    this->scheduler = scheduler;
    current_time = 0;

    for (int i = 0; i < number_of_cores; i++)
    {
        // For now:
        // first half = performance cores
        // second half = efficiency cores
        bool performance = (i < number_of_cores / 2);

        cores.emplace_back(i, performance);
    }

    if (!scheduler->controlsFrequency())
    {
        setBaselineFrequency(cores);
    }
}

void Simulator::setBaselineFrequency(
    std::vector<CPUCore>& cores)
{
    for (auto& core : cores)
    {
        core.setFrequency(3.0);
    }
}

double Simulator::run()
{
    bool processes_remaining = true;

    while (processes_remaining)
    {
        processes_remaining = false;

        // Check whether there is still work to do.
        for (const auto& process : processes)
        {
            if (process.remaining_time > 0)
            {
                processes_remaining = true;
                break;
            }
        }

        if (!processes_remaining)
            break;

        // Ask scheduler to assign waiting processes.
        scheduler->schedule(
            processes,
            cores,
            current_time
        );

        // Execute one unit of time on every busy core.
        for (auto& core : cores)
        {

            if (!core.isBusy())
            {
                core.updateUtilization(false);
                continue;
            }

            // This core was actually executing during this tick.
            core.updateUtilization(true);

            double power = PowerModel::getPower(core.getFrequency()) *
                core.getPowerFactor();

            double energy = PowerModel::calculateEnergy(power, 1.0);

            core.addEnergy(energy);



            int pid = core.getCurrentProcess();

            for (auto& process : processes)
            {
                if (process.pid != pid)
                    continue;


                
                if (process.start_time == -1)
                {
                    process.start_time = current_time;
                
                    process.response_time = process.start_time - process.arrival_time;
                }

                // Frequency is relative to the 3 GHz baseline: 3 GHz performs
                // one burst unit per tick, while lower frequencies take longer.
                const double work = core.getFrequency() / 3.0 *
                    core.getSpeedFactor();
                const double completed_work =
                    std::min(process.remaining_time, work);
                process.remaining_time -= completed_work;
                process.work_completed += completed_work;
                // Service time is elapsed CPU time, not burst work. Keeping
                // these separate is necessary when the frequency is below 3 GHz.
                process.service_time += 1.0;

                if (process.io_events_remaining > 0 &&
                    process.work_completed + 1e-9 >= process.next_io_at)
                {
                    process.io_events_remaining--;
                    process.next_io_at += process.io_work_interval;
                    process.blocked_until = current_time + 1 +
                        process.io_wait_per_event;
                    process.io_time += process.io_wait_per_event;
                    core.release();
                }
                // Process finished.
                else if (process.remaining_time <= 1e-9)
                {
                    process.remaining_time = 0.0;
                    process.completion_time =
                        current_time + 1;

                    process.turnaround_time =
                        process.completion_time -
                        process.arrival_time;

                    process.waiting_time = std::max(
                        0.0, process.turnaround_time - process.service_time -
                            process.io_time);

                    core.release();
                }
                else
                {
                    core.incrementTimeSlice();
                }

                break;
            }
        }

        current_time++;
    }



    double total_energy = 0.0;


    std::cout << "==========================================\n";
    for (const auto& core : cores)
    {
        std::cout << "Core "
                  << core.getId()
                  << " Energy: "
                  << core.getTotalEnergy()
                  << " J\n";

        total_energy += core.getTotalEnergy();
    }

    std::cout << "Total Energy: "
              << total_energy
              << " J\n";

    return total_energy;
}

int Simulator::getCurrentTime() const
{
    return current_time;
}


const std::vector<Process>& Simulator::getProcesses() const
{
    return processes;
}
