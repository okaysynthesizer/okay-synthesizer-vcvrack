// #include "AverageBuffer.h"

// #include <cstdint>

// template <class T>
// AverageBuffer<T>::AverageBuffer(uint32_t size, T defaultValue) {
//   this->bufferSize = size;
//   this->values = new T[size];
//   for(uint32_t i = 0; i < size; i++) {
//     values[i] = defaultValue;
//   }
// }

// template <class T>
// void AverageBuffer<T>::addValue(T value) {
//   values[this->circularIdx] = value;
//   this->circularIdx++;
//   this->circularIdx = this->circularIdx % this->bufferSize;
// }

// template <class T>
// T AverageBuffer<T>::getAverageValue(void) {
//   T sum = 0;
//   for(uint32_t i = 0; i < this->bufferSize; i++) {
//     sum += values[i];
//   }

//   return sum / bufferSize;
// }
