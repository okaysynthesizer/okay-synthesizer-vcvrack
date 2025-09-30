#include "util.h"
#include "DiscomfortInput.h"
#include "../../DaisySP/Source/Utility/dsp.h"
#include <cmath>

using namespace daisysp;

// Basic mapping function
float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Clamp function for local use (uses DaisySP's fclamp internally)
float clamp(float x, float min_val, float max_val) {
    return fclamp(x, min_val, max_val);
}

// Map and clamp combined - clamps input to input range, maps to output range, and clamps result
float mapClamp(float x, float in_min, float in_max, float out_min, float out_max) {
    float clamped_input = clamp(x, in_min, in_max);
    float mapped = map(clamped_input, in_min, in_max, out_min, out_max);
    return clamp(mapped, out_min, out_max);
}

// Bidirectional mapping - convert parameter (-1 to +1) to specific range and back
float parameterToRange(float param, float min_val, float max_val) {
    // Clamp parameter to -1 to +1 range first
    param = clamp(param, -1.0f, 1.0f);
    // Convert -1/+1 to 0/1, then map to range
    float normalized = (param + 1.0f) * 0.5f;
    return mapClamp(normalized, 0.0f, 1.0f, min_val, max_val);
}

// Sign function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// Explicit template instantiations for common types
template int sgn<float>(float val);
template int sgn<double>(double val);
template int sgn<int>(int val);

