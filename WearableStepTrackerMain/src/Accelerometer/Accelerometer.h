#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

/**
* Tracks and returns the acceleration detected by the accelerometer.
*   
* By Julia Behnen
* 
* Lightly adapted from the acceldemo example code referenced at https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/arduino
*/ 
class Accelerometer {
  private:
    Adafruit_LIS3DH _lis;
    float _accelerationX;
    float _accelerationY;
    float _accelerationZ;
    int _accelerationMagnitude;
    
  public:
    // Constructor
    Accelerometer(int clkPin, int misoPin, int mosiPin, int csPin);
	
    void setup(); 
    /**
    * Calculates and returns an HSL value between 0 and 1, or -1 if no appropriate
	* HSL value can be derived, based on the value from the _analogInputPin 
	* relative to the _minValue and _maxValue. 
    * Call this function once per loop() if this HSL value is required.
    */ 
    void update();
	
    float getAccelerationX();
    float getAccelerationY();
    float getAccelerationZ();
    
    int getAccelerationMagnitude();
};

#endif