#ifndef DISCOMFORT_INPUT_H
#define DISCOMFORT_INPUT_H

#include "Follower.h"
#include "Folder.h"
#include "Clipper.h"
#include "FilterBank.h"
#include "util.h"

#define INPUT_MIN_GAIN 0
#define INPUT_MAX_GAIN 5
#define OUTPUT_MIN_GAIN 0
#define OUTPUT_MAX_GAIN 5

enum DistModes {
  DIST_MODE_NOISE_PARTICLE,
  DIST_MODE_SOFT_CLIP,
  DIST_MODE_CRUSH,
  DIST_MODE_WAVEFOLDER,
};

enum RoutingModes {
  SERIAL_AB,
  SERIAL_BA,
  PARALLEL,
};

struct DistortionParams {
  float a;
  float b;
  float c;
  float mix;
};

class DiscomfortInput
{
public:
  float input;
  float inputGain;
  float outputGain;

  RoutingModes routingMode;
  DistortionParams distParamsA;
  DistortionParams distParamsB;
  DistModes distModeA;
  DistModes distModeB;

  // Follower
  float attack;
  float decay;
  float envGain;

  // Select
  int envRoutingSelection; // bitmask for env routing selection

  void setAParams(float a, float b, float c, float mix)
  {
    this->distParamsA.a = a;
    this->distParamsA.b = b;
    this->distParamsA.c = c;
    this->distParamsA.mix = mix;
  }

  void setBParams(float a, float b, float c, float mix)
  {
    this->distParamsB.a = a;
    this->distParamsB.b = b;
    this->distParamsB.c = c;
    this->distParamsB.mix = mix;
  }

  void setEnvFollowerValues(float attack, float decay, float envGain)
  {
    this->attack = map(attack, 0, 1, ENV_ATTACK_MIN, ENV_ATTACK_MAX);
    this->decay = map(decay, 0, 1, ENV_DECAY_MIN, ENV_DECAY_MAX);
    this->envGain = map(envGain, 0, 1, ENV_GAIN_MIN, ENV_GAIN_MAX);
  }

  // empty values for development
  static DiscomfortInput create()
  {
    DiscomfortInput dcInput;
    dcInput.inputGain = 1;
    dcInput.outputGain = 1;
    dcInput.distParamsA.a = 0;
    dcInput.distParamsA.b = 0;
    dcInput.distParamsA.c = 0;
    dcInput.distParamsA.mix = 0;
    dcInput.distParamsB.a = 0;
    dcInput.distParamsB.b = 0;
    dcInput.distParamsB.c = 0;
    dcInput.distParamsB.mix = 0;
    dcInput.attack = 1;
    dcInput.decay = 50;
    dcInput.envGain = 1;
    dcInput.routingMode = SERIAL_AB;
    dcInput.distModeA = DIST_MODE_WAVEFOLDER;
    dcInput.distModeB = DIST_MODE_CRUSH;
    dcInput.envRoutingSelection = 0;
    return dcInput;
  }

  static DiscomfortInput create(float audioIn)
  {
    DiscomfortInput dcInput = DiscomfortInput::create();
    dcInput.input = audioIn;
    return dcInput;
  }
};

#endif