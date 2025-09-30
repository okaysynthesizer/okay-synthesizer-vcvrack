#include "Crush.h"

Crush::Crush(float sampleRate)
{
  _sampleRate = sampleRate;
}

float Crush::crush(float input, int bitDepth, int samplingSkip)
{
  // Calculate number of quantization levels
  int levels = 1 << bitDepth; // 2^bitDepth

  // Normalize input to 0-1 range
  float normalized = (input + 1.0f) * 0.5f;

  // Quantize to the specified bit depth
  int quantized = static_cast<int>(normalized * (levels - 1) + 0.5f);

  // Convert back to -1 to 1 range
  float bitCrushed = (static_cast<float>(quantized) / (levels - 1)) * 2.0f - 1.0f;

  if(samplesSinceLastResample == 0) {
    lastSample = bitCrushed;
  }

  samplesSinceLastResample = (samplesSinceLastResample + 1) % samplingSkip;

  return lastSample;
}
