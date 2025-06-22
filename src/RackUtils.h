#pragma once
#include "discomfort/DiscomfortInternal/util.h"

float rackCvToInternal(float cvValue)
{
    return fclamp(map(cvValue, -5, 5, -1, 1), -1, 1);
}