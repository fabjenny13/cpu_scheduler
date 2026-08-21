#include "Core.h"
#include <limits>



Task::Task(int id,
           double utilization,
           double work,
           double arrivalTime)
{
    this->id = id;
    this->utilization = utilization;
    this->work = work;
    this->arrivalTime = arrivalTime;

    assignedCore = -1;
    startTime = 0;
    finishTime = 0;
    executionTime = 0;
    energy = 0;
}


Core::Core(int id)
{
    this->id = id;

    utilization = 0;
    frequency = 1.0;
    busyUntil = 0;
}

int Core::getId() const
{
    return id;
}

double Core::getUtilization() const
{
    return utilization;
}

double Core::getFrequency() const
{
    return frequency;
}

double Core::getBusyUntil() const
{
    return busyUntil;
}


void Core::addUtilization(double value)
{
    utilization += value;
}

void Core::setFrequency(double value)
{
    frequency = value;
}

void Core::setBusyUntil(double value)
{
    busyUntil = value;
}



// Baseline uses ONLY utilization.
//
// <= 30%  -> 1 GHz
// <= 70%  -> 2 GHz
// > 70%   -> 3 GHz

double chooseFrequency(double utilization)
{
    if (utilization <= 30)
        return 1.0;

    if (utilization <= 70)
        return 2.0;

    return 3.0;
}


double getPower(double frequency)
{
    if (frequency == 1.0)
        return 3.0;

    if (frequency == 2.0)
        return 6.0;

    if (frequency == 3.0)
        return 12.0;

    return 0.0;
}

// Baseline chooses the least-utilized
// core on which the task can fit.

int chooseCore(
    const std::vector<Core>& cores,
    const Task& task)
{
    int bestCore = -1;

    double lowestUtilization =
        std::numeric_limits<double>::max();

    for (int i = 0; i < cores.size(); i++)
    {
        double newUtilization =
            cores[i].getUtilization()
            + task.utilization;

        // Task can fit on this core
        if (newUtilization <= 100)
        {
            if (cores[i].getUtilization()
                < lowestUtilization)
            {
                lowestUtilization =
                    cores[i].getUtilization();

                bestCore = i;
            }
        }
    }

    return bestCore;
}
