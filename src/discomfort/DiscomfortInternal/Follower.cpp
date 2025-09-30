#include <cmath>
#include <algorithm>
#include "Follower.h"

#define DECAY_FACTOR 2.f

Follower::Follower(float sampleRate)
{
    this->sampleRate = sampleRate;
    this->envelope = 0.f;
}

float Follower::process(float x, float sensitivity, float attackMs, float decayMs)
{
    float absx = std::fabs(x) * sensitivity;

    // Convert to base coefficients
    float attackCoeffBase = std::exp(std::log(0.01f) / (sampleRate * (attackMs * 0.001f)));
    float decayCoeffBase = std::exp(std::log(0.01f) / (sampleRate * (decayMs * 0.001f)));

    if (absx > envelope)
    {
        float levelFactor = daisysp::fclamp(absx - envelope, 0.0f, 1.0f);
        float curvedAttack = std::pow(attackCoeffBase, 1.0f + levelFactor * 3.f);
        envelope = curvedAttack * envelope + (1.f - curvedAttack) * absx;
    }
    else
    {
        float levelFactor = daisysp::fclamp(envelope, 0.0f, 1.0f);
        float curvedDecay = std::pow(decayCoeffBase, 1.0f + levelFactor * 3.f);
        envelope = curvedDecay * envelope + (1.f - curvedDecay) * absx;
    }

    return envelope;
}