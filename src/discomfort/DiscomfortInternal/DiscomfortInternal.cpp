#include <cmath>

#include "Clipper.h"
#include "Crush.h"
#include "DiscomfortInput.h"
#include "DiscomfortInternal.h"
#include "DryWet.h"
#include "Folder.h"

DiscomfortInternal::DiscomfortInternal() {
  this->follower = nullptr;
  this->folder = nullptr;
  this->filterBank = nullptr;
  this->crush = nullptr;
}

void DiscomfortInternal::init(float sampleRate) {
  this->follower = new Follower(sampleRate);
  this->folder = new Folder(sampleRate);
  this->filterBank = new FilterBank(sampleRate);
  this->crush = new Crush(sampleRate);
  noise.Init();
  noiseParticle.Init(sampleRate);
}

float DiscomfortInternal::processDistortion(float audioIn, DistModes mode,
                                            const DistortionParams &params) {
  float output = audioIn;

  switch (mode) {

  case DIST_MODE_WAVEFOLDER: {
    output = folder->fold(
      audioIn,
      parameterToRange(params.b, FOLDER_MIN_GAIN, FOLDER_MAX_GAIN),
      parameterToRange(params.a, FOLDER_MIN_TIMBRE, FOLDER_MAX_TIMBRE),
      0
    );
    break;
  }

  case DIST_MODE_SOFT_CLIP: {
    output = Clipper::clip(audioIn, params.c, params.b, params.a);
    break;
  }

  case DIST_MODE_NOISE_PARTICLE: {
    noiseParticle.SetSpread(map(fclamp(params.a, 0, 1), 0, 1, 0, 100));
    noiseParticle.SetResonance(fclamp(params.b, 0.1, 1));
    noiseParticle.SetFreq(map(params.c, 0, 1, 20, 10000));
    output = audioIn * noiseParticle.Process();
    break;
  }

  case DIST_MODE_CRUSH: {
    output = crush->crush(
      audioIn,
      CRUSH_BIT_DEPTH + 1 - map(fclamp(params.a, 0, 1), 0, 1, 1, CRUSH_BIT_DEPTH),
      std::floorf(map(params.b, 0, 1, 1, SRR_MAX_SAMPLING_SKIP))
    );
    break;
  }

  default:
    break;
  }

  return DryWet::blend(audioIn, output, params.mix);
}

float DiscomfortInternal::getA(float audioIn, DiscomfortInput input) {
  return processDistortion(audioIn, input.distModeA, input.distParamsA);
}

float DiscomfortInternal::getB(float audioIn, DiscomfortInput input) {
  return processDistortion(audioIn, input.distModeB, input.distParamsB);
}

DiscomfortOutput DiscomfortInternal::process(DiscomfortInput input) {
  float gainStagedInput = input.input * input.inputGain;
  float followerAmplitude =
      this->follower->process(gainStagedInput, input.envGain, input.attack, input.decay);

  float output = 0.f;

  if (input.routingMode == SERIAL_AB)
  {
    float a = this->getA(gainStagedInput, input);
    output = this->getB(a, input);
  }

  if (input.routingMode == SERIAL_BA)
  {
    float b = this->getB(gainStagedInput, input);
    output = this->getA(b, input);
  }

  if (input.routingMode == PARALLEL)
  {
    float a = this->getA(gainStagedInput, input) * 0.5;
    float b = this->getB(gainStagedInput, input) * 0.5;
    output = a + b;
  }

  // output = this->getA(gainStagedInput, input);
  // output = this->getB(output, input);

  output = output * input.outputGain;

  return this->createOutput(output, followerAmplitude);
}

DiscomfortOutput DiscomfortInternal::createOutput(float audio, float follower) {
  DiscomfortOutput dcOutput;
  dcOutput.audioOutput = audio;
  dcOutput.followerOutput = follower;
  return dcOutput;
}
