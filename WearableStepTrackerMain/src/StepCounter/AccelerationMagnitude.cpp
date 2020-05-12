#include "AccelerationMagnitude.h"

AccelerationMagnitude :: AccelerationMagnitude(int bufferWindowSize) :
  _bufferWindowSize(bufferWindowSize)  {
  _bufferedAcceleration[bufferWindowSize][3] = {0};
  _bufferedAccelerationIndex = 0;
}

void AccelerationMagnitude :: update(sensors_vec_t acceleration, long timestamp) {
  
}


