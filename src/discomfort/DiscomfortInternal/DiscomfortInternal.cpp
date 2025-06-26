#include <cmath>

#include "DiscomfortInternal.h"
#include "Folder.h"
#include "Clipper.h"
#include "DryWet.h"
#include "DiscomfortInput.h"

DiscomfortInternal::DiscomfortInternal() {}

void DiscomfortInternal::init(float sampleRate)
{
  this->follower = new Follower(sampleRate, 1.0f, 1.0f);
  this->filterBank = new FilterBank(sampleRate);
  noise.Init();
  noiseParticle.Init(sampleRate);
}

float DiscomfortInternal::getFold(float audioIn, DiscomfortInput input)
{
  float foldOut = Folder::fold(audioIn, input.foldGain, input.foldOffset, input.foldSymmetry);
  return DryWet::blend(audioIn, foldOut, pow(input.foldDryWet, 2));
}

float DiscomfortInternal::getDist(float audioIn, DiscomfortInput input)
{
  // if(DIST_MODE_NOISE_PARTICLE) {
  //   // NOTE PLAY WITH DENSITY
  //   noiseParticle.SetSpread(map(fclamp(input.distA, 0, 1), 0, 1, 0, 100));
  //   noiseParticle.SetResonance(fclamp(input.distB, 0.1, 1));
  //   noiseParticle.SetFreq(map(input.distC, 0, 1, 20, 10000));
  //   float noiseOut = noiseParticle.Process();
  //   float noiseBlend = foldBlend + (noiseOut * input.distDryWet);
  //   output = noiseBlend * input.outputGain;
  // }
  if(input.distMode == DIST_MODE_SOFT_CLIP) {
    float clippedOut = Clipper::clip(audioIn, input.distC, input.distB, input.distA);
    return DryWet::blend(audioIn, clippedOut, input.distDryWet);
  }

  if(input.distMode == DIST_MODE_NOISE_PARTICLE) {
    noiseParticle.SetSpread(map(fclamp(input.distA, 0, 1), 0, 1, 0, 100));
    noiseParticle.SetResonance(fclamp(input.distB, 0.1, 1));
    noiseParticle.SetFreq(map(input.distC, 0, 1, 20, 10000));
    return DryWet::blend(audioIn, audioIn * noiseParticle.Process(), input.distDryWet);
  }
}

DiscomfortOutput DiscomfortInternal::process(DiscomfortInput input)
{
  float gainStagedInput = input.input * input.inputGain;
  float followerAmplitude = this->follower->process(gainStagedInput * input.envGain, input.attack, input.decay);

  float output = 0;

  if(input.routingMode == SERIAL_AB) {
    float foldOut = this->getFold(gainStagedInput, input);
    output = this->getDist(foldOut, input);
  }

  if(input.routingMode == SERIAL_BA) {
    float distOut = this->getDist(gainStagedInput, input);
    output = this->getFold(distOut, input);
  }

  if(input.routingMode == PARALLEL) {
    float foldOut = this->getFold(gainStagedInput, input);
    float distOut = this->getDist(gainStagedInput, input);
    output = foldOut + distOut;
  }

  output = output * input.outputGain;

  return this->createOutput(output, followerAmplitude);

  // return this->createOutput(output * 0.7, followerAmplitude);
}

DiscomfortOutput DiscomfortInternal::createOutput(float audio, float follower)
{
  DiscomfortOutput dcOutput;
  dcOutput.audioOutput = audio;
  dcOutput.followerOutput = follower;
  return dcOutput;
}
