#ifndef DISCOMFORT_UTIL_H
#define DISCOMFORT_UTIL_H

/*
 * Discomfort Utility Functions
 * 
 * This header provides comprehensive mapping and clamping utilities for 
 * converting between different value ranges with automatic bounds checking.
 * 
 * Example Usage:
 * 
 * // Basic mapping with input and output clamping:
 * float volume = mapClamp(knobValue, 0.0f, 1.0f, 0.0f, 100.0f);  // Clamps input to 0-1, maps to 0-100 volume
 * 
 * // Parameter (-1 to +1) to specific range:
 * float frequency = parameterToRange(pitchKnob, 20.0f, 20000.0f);  // Maps -1/+1 to 20Hz-20kHz
 * 
 * // Range back to parameter:
 * float knobPos = rangeToParameter(440.0f, 20.0f, 20000.0f);  // Convert 440Hz back to knob position
 * 
 * // Input/Output gain mapping (uses constants from DiscomfortInput.h):
 * float inputGain = mapInputGain(gainKnob);   // Maps -1/+1 to INPUT_MIN_GAIN/INPUT_MAX_GAIN
 * float outputGain = mapOutputGain(outKnob);  // Maps -1/+1 to OUTPUT_MIN_GAIN/OUTPUT_MAX_GAIN
 */

// Basic mapping function
float map(float x, float in_min, float in_max, float out_min, float out_max);

// Clamp function for local use (uses DaisySP's fclamp internally)
float clamp(float x, float min_val, float max_val);

// Map and clamp combined - clamps input to input range, maps to output range, and clamps result
float mapClamp(float x, float in_min, float in_max, float out_min, float out_max);

// Bidirectional mapping - convert parameter (-1 to +1) to specific range and back
float parameterToRange(float param, float min_val, float max_val);

// Sign function
template <typename T> int sgn(T val);

#endif