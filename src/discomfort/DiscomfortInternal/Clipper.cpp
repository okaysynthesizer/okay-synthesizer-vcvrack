#include <cmath>
#include "Clipper.h"
#include "DryWet.h"
#include "util.h"

float softClip(float y) {
  return tanh(y);
}

float hardClip(float y) {
  if (y >= 0.8f) {
    return 0.8f;
  }

  if (y <= -0.8f) {
    return -0.8f;
  }

  return y;
}

float sinClip(float y) {
  return sin(y);
}

#define THRESHOLD_CLIPBEND 0.49999999999

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float Clipper::clip(float input, float gain, float bend) {
  input *= gain;
  // https://www.musicdsp.org/en/latest/Effects/104-variable-hardness-clipping-function.html
  return sgn(input) * pow (atan (pow (abs(input), bend)), (1 / bend));


  // float y = input * gain;

  // float softClipOut = softClip(y);
  // float hardClipOut = hardClip(y);
  // float sinClipOut = sinClip(y);


  // return hardClipOut;
  // return DryWet::blend(softClipOut, hardClipOut, bend);

  // return softClipOut;

  // float newBend;

  // if(bend <= THRESHOLD_CLIPBEND) {
  //   newBend = map(bend, 0, THRESHOLD_CLIPBEND, 0, 1);
  //   return softClipOut + (newBend * (hardClipOut - softClipOut));
  // } else {
  //   newBend = map(bend, 0.5, 0.1, 0, 1);
  //   return hardClipOut + (newBend * (sinClipOut - hardClipOut));
  // } 
}