#include "FilterBank.h"
#include "util.h"

#include "daisysp.h"
using namespace daisysp;

#define FILTERBANK_GAIN_FACTOR 7.0f

FilterBankBand::FilterBankBand(float sampleRate, float lowFreq, float highFreq) {
  lowPass.Init(sampleRate);
  lowPass.SetFreq(highFreq);
  lowPass.SetRes(0.1);
  lowPass.SetDrive(0);

  highPass.Init(sampleRate);
  highPass.SetFreq(lowFreq);
  highPass.SetRes(0.1);
  highPass.SetDrive(0);
}

float FilterBankBand::process(float input, float level) {
  float gain = 1;
  highPass.SetDrive(0);
  lowPass.SetDrive(0);
  
  gain = level;

  if (level < 0.75) {
    gain = map(level, 0.0f, 0.75f, 0.0f, 1.0f);
  } else {
    float drive = map(1.0f - level, 0.0f, 0.25f, 0.0f, 20.0f);
    highPass.SetDrive(drive);
    lowPass.SetDrive(drive);
  }

  highPass.Process(input);
  lowPass.Process(highPass.High());
  float output = lowPass.Low();

  return output * gain;
}

FilterBank::FilterBank(float sampleRate) {
  bandA = new FilterBankBand(sampleRate, 20, 400);
  bandB = new FilterBankBand(sampleRate, 400, 2000);
  bandC = new FilterBankBand(sampleRate, 2000, 10000);
  bandD = new FilterBankBand(sampleRate, 10000, 20000);
}

float FilterBank::process(
  float input,
  FilterBankType type,
  float levelA,
  float levelB,
  float levelC,
  float levelD
) {
  if (type == FILTERBANK_OFF) {
    return input;
  }

  float outA = bandA->process(input, levelA);
  float outB = bandB->process(input, levelB);
  float outC = bandC->process(input, levelC);
  float outD = bandD->process(input, levelD);

  return outA + outB + outC + outD;
  // return outD;
}
