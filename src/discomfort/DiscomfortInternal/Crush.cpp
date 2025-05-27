// #include "Crush.h"

// Crush::Crush(float sampleRate) {
//   _sampleRate = sampleRate;
//   bitcrush.Init(sampleRate);
// }

// float Crush::crush(float input, float amount) {
//   float rate = _sampleRate * (1 - (amount * amount));
//   bitcrush.SetCrushRate(rate);
//   return bitcrush.Process(input);
// }