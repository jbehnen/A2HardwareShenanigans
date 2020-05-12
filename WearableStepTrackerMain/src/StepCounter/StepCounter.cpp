#include "StepCounter.h"
#include <cmath>
#include <Arduino.h>

StepCounter :: StepCounter(int minDistanceBetweenPeaksMs, int minPeakHeight, int bufferWindowSize) :
	_minDistanceBetweenPeaksMs(minDistanceBetweenPeaksMs), 
  _minPeakHeight(minPeakHeight),
  _bufferWindowSize(bufferWindowSize) {
  _steps = 0;
  _initialValueCount = 0;  
  _lastPeakTimestamp = -1;
  _bufferedAccelerationMags[_bufferWindowSize] = {0};
  _bufferedAccelerationTimestamps[_bufferWindowSize] = {0};
  _bufferedAccelerationIndex = 0;
}

void StepCounter :: update(int accelerationMag, long timestamp) {
  _bufferedAccelerationMags[_bufferedAccelerationIndex] = accelerationMag;
  _bufferedAccelerationTimestamps[_bufferedAccelerationIndex] = timestamp;
  
  Serial.print("Initial value count: ");
  Serial.println(_initialValueCount);
  Serial.print("Buffer window size: ");
  Serial.println(_bufferWindowSize);
  
  if (_initialValueCount < 50) {
    _initialValueCount++;
    return;
  }
  
  // TODO: detrend (ongoing average?)
  
  
  // TODO: smooth (ongoing)
  
  // for (int i = 1; i < _bufferWindowSize - 1; i++) {
  for (int i = _bufferedAccelerationIndex + 2;
     i < _bufferedAccelerationIndex + 50; 
     i++) {
    int index = wrapIndex(i);
    int previousIndex = wrapIndex(i - 1);
    int nextIndex = wrapIndex(i + 1);
    int value = _bufferedAccelerationMags[index];
    int previousValue = _bufferedAccelerationMags[previousIndex];
    int nextValue = _bufferedAccelerationMags[nextIndex];
    
    int forwardSlope = nextValue - value;
    int backwardSlope = value - previousValue;
    
    if (forwardSlope < 0 and backwardSlope > 0) {
      long peakVal = _bufferedAccelerationMags[index];
      if (peakVal > _minPeakHeight) {
        long timeSinceLastDetectionMs = 
          _bufferedAccelerationTimestamps[index] - _lastPeakTimestamp;
        if (_lastPeakTimestamp == -1 || 
            timeSinceLastDetectionMs > _minDistanceBetweenPeaksMs) {
            _lastPeakTimestamp = _bufferedAccelerationTimestamps[index];
            _steps++;
        }
      }
    }
  }
  
  _bufferedAccelerationIndex = (_bufferedAccelerationIndex + 1) % 50;
}

int StepCounter :: getSteps() {
  return _steps;
}

// Private

int StepCounter :: wrapIndex(int i) {
  return i % 50;
}