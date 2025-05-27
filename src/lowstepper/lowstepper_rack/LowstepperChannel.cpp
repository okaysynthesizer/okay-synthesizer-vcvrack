#include "LowstepperChannel.h"

const float eocHighOutVoltage = 10.f;
const float eocHighSampleLength = 2000.f;

Channel::Channel(float sampleRate)
    : lfo(sampleRate)
{
}

void Channel::tickEoc(void)
{
    if (this->lastOutput.eocGateHigh)
    {
        this->eocHigh = true;
    }

    if (this->eocSampleTime > eocHighSampleLength)
    {
        this->eocSampleTime = 0;
        this->eocHigh = false;
    }

    if (this->eocHigh)
    {
        this->eocVoltageOutput = eocHighOutVoltage;
        this->eocSampleTime++;
    }
    else
    {
        this->eocVoltageOutput = 0;
    }
}