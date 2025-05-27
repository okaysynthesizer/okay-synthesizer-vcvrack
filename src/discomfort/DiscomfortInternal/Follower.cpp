#include <cmath>
#include "Follower.h"

/**
 * Big thanks to the following article for this code
 * https://kferg.dev/posts/2020/audio-reactive-programming-envelope-followers/
 */

float msToSamples(float sampleRate, float ms) {
  float samples = (
    sampleRate *
    (ms / 1000.0)
  );
  return exp(log(0.5) / samples);
}

Follower::Follower(float sampleRate, float attackMs, float decayMs) {
  this->sampleRate = sampleRate;
  this->a_ = msToSamples(this->sampleRate, attackMs);
  this->b_ = msToSamples(this->sampleRate, decayMs);
  this->y_ = 0;
}

float Follower::process(float x, float attackMs, float decayMs) {
  // this->a_ = msToSamples(this->sampleRate, attackMs);
  // this->b_ = msToSamples(this->sampleRate, decayMs);
  this->a_ = attackMs;
  this->b_ = decayMs;
  const auto abs_x = abs(x);
  if (abs_x > y_) {
    y_ = a_ * y_ + (1 - a_) * abs_x;
  } else {
    y_ = b_ * y_ + (1 - b_) * abs_x;
  }
  return y_;
}
