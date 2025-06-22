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

#define FOLD_MODE_1 1

enum DistModes {
  DIST_MODE_NOISE_PARTICLE,
  DIST_MODE_SOFT_CLIP,
};

enum RoutingModes {
  SERIAL_AB,
  SERIAL_BA,
  PARALLEL,
};

class DiscomfortInput
{
public:
  float input;
  float inputGain;
  float outputGain;

  // wavefolder
  float foldGain;     // 0 ... N
  float foldOffset;   // -1 ... +1
  float foldSymmetry; // -1 ... +1

  // Distortion
  float distA;
  float distB;
  float distC;
  ClipperType clipperType;

  // Crush
  float crushValue;

  // Follower
  float attack;
  float decay;
  float envGain;

  // Dry/Wet
  float foldDryWet;
  float distDryWet;

  RoutingModes routingMode;
  DistModes distMode;

  void setGainValues(float inputGain, float outputGain)
  {
    this->inputGain = map(inputGain, 0, 1, INPUT_MIN_GAIN, INPUT_MAX_GAIN);
    this->outputGain = map(outputGain, 0, 1, OUTPUT_MIN_GAIN, OUTPUT_MAX_GAIN);
  }

  void setFolderValues(float gain, float offset, float symmetry, float dryWet)
  {
    this->foldGain = map(gain, 0, 1, FOLDER_MIN_GAIN, FOLDER_MAX_GAIN);
    this->foldOffset = map(offset, 0, 1, FOLDER_MIN_OFFSET, FOLDER_MAX_OFFSET);
    this->foldSymmetry = map(symmetry, 0, 1, FOLDER_MIN_SYMMETRY, FOLDER_MAX_SYMMETRY);
    this->foldDryWet = dryWet;
  }

  void setDistValues(float a, float b, float c, float mix)
  {
    this->distA = a;
    this->distB = b;
    this->distC = c;
    this->distDryWet = mix;
  }

  void setEnvFollowerValues(float attack, float decay, float envGain)
  {
    this->attack = map(attack, 0, 1, ENV_ATTACK_MIN, ENV_ATTACK_MAX);
    this->decay = map(decay, 0, 1, ENV_DECAY_MIN, ENV_DECAY_MAX);
    this->envGain = map(envGain, 0, 1, ENV_GAIN_MIN, ENV_GAIN_MAX);
  }

  // void setEnvelopeValues(float gain, float attack, float decay) {
  //   this->envGain = 1;
  //   this->attack = 1;
  //   this->decay = 1;
  // }

  // empty values for development
  static DiscomfortInput create()
  {
    DiscomfortInput dcInput;
    dcInput.inputGain = 1;
    dcInput.outputGain = 1;
    dcInput.foldGain = FOLDER_MIN_GAIN;
    dcInput.foldOffset = 0;
    dcInput.foldSymmetry = 0;
    dcInput.distC = 0;
    dcInput.distA = 0;
    dcInput.distB = 0;
    dcInput.clipperType = CLIPPER_SOFT;
    dcInput.crushValue = 0;
    dcInput.attack = 1;
    dcInput.decay = 50;
    dcInput.envGain = 1;
    dcInput.foldDryWet = 0;
    dcInput.distDryWet = 0;
    dcInput.routingMode = SERIAL_AB;
    dcInput.distMode = DIST_MODE_SOFT_CLIP;
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