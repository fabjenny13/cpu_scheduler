#ifndef FREQUENCY_H
#define FREQUENCY_H

enum class FrequencyLevel
{
    LOW,
    MEDIUM,
    HIGH
};

double getFrequencyGHz(FrequencyLevel level);

#endif