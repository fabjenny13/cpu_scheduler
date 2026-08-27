#ifndef POWER_MODEL_H
#define POWER_MODEL_H

class PowerModel
{
public:
    static double getPower(double frequencyGHz);

    static double calculateEnergy(
        double power,
        double executionTime
    );
};

#endif