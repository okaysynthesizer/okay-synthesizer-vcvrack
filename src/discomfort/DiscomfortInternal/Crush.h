#ifndef DISCOMFORT_CRUSH_H
#define DISCOMFORT_CRUSH_H

#include "../../DaisySP/Source/daisysp.h"
using namespace daisysp;

#define CRUSH_BIT_DEPTH 16
#define SRR_MAX_SAMPLING_SKIP 128

class Crush {
  public:
    Crush(float sampleRate);
    float crush(float input, int bitDepth, int samplingSkip);
  private:
    float _sampleRate;
    float lastSample = 0;
    int samplesSinceLastResample = 0;
};

#endif
