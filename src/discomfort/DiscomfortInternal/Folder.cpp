#include <cmath>
#include "Folder.h"

float Folder::fold(float input, float gain, float offset, float symmetry) {
  float sign = 1;
  input -= offset;
  if(input < 0) {
    sign = -1;
    input = abs(input);
  }

  float newInput;
  float s = 2;
  float z = s * abs(symmetry);
  if (symmetry > 0 && sign == 1) {
    gain = gain * (1 + abs(symmetry));
  } else if(symmetry < 0 && sign == -1) {
    gain = gain * (1 + abs(symmetry));
  }

  newInput = input * gain;

  // input * gain does not exceed folding threshold
  // no need to compute anything further 
  if (newInput <= 1) {
    return newInput * sign;
  }

  int numberOfFolds = (int) floor((newInput + 1) / 2);
  numberOfFolds = numberOfFolds == 0 ? 1 : numberOfFolds;
  
  float foldValue = 0;
  float sub = numberOfFolds == 1 ? numberOfFolds : (numberOfFolds * 2.0f - 1);
  if(numberOfFolds % 2 == 0) {
    foldValue = -1 + (newInput - sub);
  } else {
    foldValue = 1 - (newInput - sub);
  }

  return foldValue * sign;
}