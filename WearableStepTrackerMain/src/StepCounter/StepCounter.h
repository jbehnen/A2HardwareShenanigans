#ifndef STEP_COUNTER_H
#define STEP_COUNTER_H

#include <Adafruit_Sensor.h>

/**
* Updates the OLED display.
*   
* By Julia Behnen
* 
* Adapted from class resource StepTracker-Exercises.ipynb
*/ 
class StepCounter {
  private:
    const int _minDistanceBetweenPeaksMs;
	  const int _minPeakHeight; // Acceleration magnitude based on m/s**2
    const int _bufferWindowSize;
  
    int _steps;
    int _initialValueCount;
    long _lastPeakTimestamp;
    long _bufferedAccelerationMags[];
    long _bufferedAccelerationTimestamps[];
    int _bufferedAccelerationIndex;
    
  public:
    // Constructor
    StepCounter(
      const int minDistanceBetweenPeaksMs, 
      const int minPeakHeight, 
      const int bufferWindowSize);
	
    void update(int accelerationMag, long timestamp);
  
    int getSteps();
    
    // TODO: make private
    
    int wrapIndex(int i);
};

#endif