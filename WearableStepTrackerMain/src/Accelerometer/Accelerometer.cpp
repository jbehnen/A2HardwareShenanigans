#include "Accelerometer.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Accelerometer :: Accelerometer(int clkPin, int misoPin, int mosiPin, int csPin) {
  _lis = Adafruit_LIS3DH();
  //_lis = Adafruit_LIS3DH(csPin, mosiPin, misoPin, clkPin);
}

void Accelerometer :: setup() {
  Serial.println("LIS3DH test!");

  if (! _lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  _lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << _lis.getRange());
  Serial.println("G");

  _lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  Serial.print("Data rate set to: ");
  switch (_lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
  }
 
}

void Accelerometer :: update() {
   // _lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  // Serial.print("X:  "); Serial.print(lis.x);
  // Serial.print("  \tY:  "); Serial.print(lis.y);
  // Serial.print("  \tZ:  "); Serial.print(lis.z);

  /* Or....get a new sensor event, normalized */
  sensors_event_t event;
  _lis.getEvent(&event);
  
  _accelerationX = event.acceleration.x;
  _accelerationY = event.acceleration.y;
  _accelerationZ = event.acceleration.z;
  _accelerationMagnitude = floor(sqrt(
      pow(_accelerationX, 2) 
    + pow(_accelerationY, 2) 
    + pow(_accelerationZ, 2)));

  /* Display the results (acceleration is measured in m/s^2) */
  
  //Serial.print(event.acceleration.x);
  //Serial.print(" X "); 
  //Serial.print(event.acceleration.y);
  //Serial.print(" Y "); 
  //Serial.print(event.acceleration.z);
  //Serial.println(" Z "); 

  // Serial.println(" m/s^2 ");
}

float Accelerometer :: getAccelerationX() {
  return _accelerationX;
}

float Accelerometer :: getAccelerationY() {
  return _accelerationY;
}

float Accelerometer :: getAccelerationZ() {
  return _accelerationZ;
}

int Accelerometer :: getAccelerationMagnitude() {
  return _accelerationMagnitude;
}