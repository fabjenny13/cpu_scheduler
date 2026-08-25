#include "Frequency.h"

double getFrequencyGHz(FrequencyLevel level)
{
    switch (level)
    {
        case FrequencyLevel::LOW:
            return 1.0;

        case FrequencyLevel::MEDIUM:
            return 2.0;

        case FrequencyLevel::HIGH:
            return 3.0;
    }

    return 1.0 ;
}