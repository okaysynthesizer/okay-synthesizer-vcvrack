#ifndef DISCOMFORT_H
#define DISCOMFORT_H

#include "daisysp.h"
#include "Follower.h"
#include "Clipper.h"
#include "FilterBank.h"
#include "DiscomfortInput.h"

using namespace daisysp;

struct DiscomfortOutput {
  float audioOutput;
  float followerOutput;
};

class DiscomfortInternal {
  public:
    DiscomfortInternal();
    void init(float sampleRate);
    DiscomfortOutput process(DiscomfortInput input);
    static DiscomfortOutput createOutput(float audio, float follower);
  private:
    Follower *follower;
    FilterBank *filterBank;
    WhiteNoise noise;
    Particle noiseParticle;
};

#endif