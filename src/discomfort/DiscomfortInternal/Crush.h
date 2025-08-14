#ifndef DISCOMFORT_CRUSH_H
#define DISCOMFORT_CRUSH_H

#include "daisysp.h"
using namespace daisysp;

#define CRUSH_BIT_DEPTH 16

class Crush {
  public:
    Crush(float sampleRate);
    float crush(float input, int bitDepth);
  private:
    float _sampleRate;
};

#endif
