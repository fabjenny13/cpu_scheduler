#include "CPUCore.h"

CPUCore::CPUCore(int id, bool performance_core)
{
    this->id = id;
    this->performance_core = performance_core;

    busy = false;
    current_process = -1;

    frequency = 1.0;

    utilization = 0.0;

    total_energy = 0.0;
    time_slice = 0;
}

int CPUCore::getId() const
{
    return id;
}

bool CPUCore::isBusy() const
{
    return busy;
}

int CPUCore::getCurrentProcess() const
{
    return current_process;
}

double CPUCore::getFrequency() const
{
    return frequency;
}

bool CPUCore::isPerformanceCore() const
{
    return performance_core;
}

void CPUCore::assignProcess(int pid)
{
    current_process = pid;
    busy = true;
    time_slice = 0;
}

void CPUCore::release()
{
    current_process = -1;
    busy = false;
}

void CPUCore::setFrequency(double frequency)
{
    this->frequency = frequency;
}

\
double CPUCore::getUtilization() const
{
    return utilization;
}

void CPUCore::updateUtilization(bool wasBusy)
{
    utilization_history.push_back(wasBusy);

    if (utilization_history.size() > UTILIZATION_WINDOW)
    {
        utilization_history.pop_front();
    }

    int busy_ticks = 0;

    for (bool busy : utilization_history)
    {
        if (busy)
            busy_ticks++;
    }

    utilization =
        (static_cast<double>(busy_ticks) /
         utilization_history.size()) * 100.0;
}

double CPUCore::getTotalEnergy() const
{
    return total_energy;
}

void CPUCore::addEnergy(double energy)
{
    total_energy += energy;
}


int CPUCore::getTimeSlice() const
{
    return time_slice;
}

void CPUCore::incrementTimeSlice()
{
    time_slice++;
}

void CPUCore::resetTimeSlice()
{
    time_slice = 0;
}