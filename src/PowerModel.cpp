#include "PowerModel.h"

double PowerModel::getPower(double frequencyGHz)
{
    if (frequencyGHz == 1.0)
        return 3.0;

    if (frequencyGHz == 2.0)
        return 6.0;

    if (frequencyGHz == 3.0)
        return 12.0;

    return 0.0;
}

double PowerModel::calculateEnergy(
    double power,
    double executionTime)
{
    return power * executionTime;
}