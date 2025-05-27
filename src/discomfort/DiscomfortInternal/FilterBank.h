#ifndef DISCOMFORT_FILTERBANK_H
#define DISCOMFORT_FILTERBANK_H

#include "daisysp.h"
using namespace daisysp;

enum FilterBankType { FILTERBANK_OFF, FILTERBANK_ON };

class FilterBankBand {
public:
  FilterBankBand(float sampleRate, float lowFreq, float highFreq);
  float process(float input, float level);
  Svf lowPass, highPass;
};

class FilterBank {
public:
  FilterBank(float sampleRate);
  float process(float input, FilterBankType type, float levelA, float levelB, float levelC, float levelD);
 
private:
  FilterBankBand *bandA, *bandB, *bandC, *bandD;
};

#endif