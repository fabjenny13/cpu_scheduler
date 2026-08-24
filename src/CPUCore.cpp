#include "CPUCore.h"

CPUCore::CPUCore(int id, bool performance_core)
{
    this->id = id;
    this->performance_core = performance_core;

    busy = false;
    current_process = -1;

    frequency = 1.0;

    utilization = 0.0;
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

void CPUCore::addUtilization(double value)
{
    utilization += value;
}

void CPUCore::removeUtilization(double value)
{
    utilization -= value;

    if (utilization < 0)
        utilization = 0;
}