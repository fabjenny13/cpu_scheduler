#include "Simulator.h"
#include "PowerModel.h"
#include <iostream>

Simulator::Simulator(
    const std::vector<Process>& processes,
    int number_of_cores,
    Scheduler* scheduler)
{
    this->processes = processes;

    for (auto& process : this->processes)
    {
        process.remaining_time = process.burst_time;
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

void Simulator::run()
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

            double power = PowerModel::getPower(core.getFrequency());

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

                process.remaining_time--;

                // Process finished.
                if (process.remaining_time == 0)
                {
                    process.completion_time =
                        current_time + 1;

                    process.turnaround_time =
                        process.completion_time -
                        process.arrival_time;

                    process.waiting_time =
                        process.turnaround_time -
                        process.burst_time;

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
}

int Simulator::getCurrentTime() const
{
    return current_time;
}


const std::vector<Process>& Simulator::getProcesses() const
{
    return processes;
}