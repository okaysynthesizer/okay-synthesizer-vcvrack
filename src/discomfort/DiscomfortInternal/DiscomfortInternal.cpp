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

DiscomfortOutput DiscomfortInternal::process(DiscomfortInput input)
{
  // Add envelope gain!!!
  float gainStagedInput = input.input * input.inputGain;
  float followerAmplitude = this->follower->process(gainStagedInput * input.envGain, input.attack, input.decay);

  float foldOut = Folder::fold(gainStagedInput, input.foldGain, input.foldOffset, input.foldSymmetry);
  float foldBlend = DryWet::blend(gainStagedInput, foldOut, pow(input.foldDryWet, 2));

  noiseParticle.SetSpread(map(fclamp(input.distA, 0, 1), 0, 1, 0, 100));
  noiseParticle.SetResonance(fclamp(input.distB, 0, 1));
  noiseParticle.SetFreq(map(input.distC, 0, 1, 20, 10000));
  float noiseOut = noiseParticle.Process();
  // float noiseOut = noiseParticle.GetNoise();
  // this->noise.SetAmp(input.distC);
  // float n = this->noise.Process();

  // float clippedOut = Clipper::clip(foldBlend, input.clipperGain, input.clipperBend);
  // float noiseBlend = DryWet::blend(foldBlend, noiseOut, pow(input.distDryWet, 2));
  float noiseBlend = foldBlend + (noiseOut * input.distDryWet);
  float output = noiseBlend * input.outputGain;
  // return this->createOutput(output * 0.7, followerAmplitude);

  return this->createOutput(output, followerAmplitude);
}

DiscomfortOutput DiscomfortInternal::createOutput(float audio, float follower)
{
  DiscomfortOutput dcOutput;
  dcOutput.audioOutput = audio;
  dcOutput.followerOutput = follower;
  return dcOutput;
}
