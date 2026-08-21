#include "Simulator.h"

Simulator::Simulator(
    const std::vector<Process>& processes,
    int number_of_cores,
    Scheduler* scheduler)
{
    this->processes = processes;
    this->scheduler = scheduler;
    current_time = 0;

    for (int i = 0; i < number_of_cores; i++)
    {
        cores.emplace_back(i);
    }
}

void Simulator::run()
{
    scheduler->schedule(processes);
}

int Simulator::getCurrentTime() const
{
    return current_time;
}