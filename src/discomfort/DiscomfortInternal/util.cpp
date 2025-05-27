#include "util.h"
#include <cmath>

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
