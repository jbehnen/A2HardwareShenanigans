#ifndef ACCELERATION_MAGNITUDE_H
#define ACCELERATION_MAGNITUDE_H

#include <Adafruit_Sensor.h>

/**
*   
* By Julia Behnen
* 
*/ 
class AccelerationMagnitude {
  private:
    const int _bufferWindowSize;
  
    float _bufferedAcceleration[][3];
    int _bufferedAccelerationIndex;
    
  public:
    // Constructor
    AccelerationMagnitude(int bufferWindowSize);
	
    void update(sensors_vec_t acceleration, long timestamp);
};

#endif