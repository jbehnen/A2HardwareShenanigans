/**
 * 
 * By Julia Behnen
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
// #include "src/Accelerometer/Accelerometer.h"
// #include "src/Oled/Oled.h"
#include "src/StepCounter/StepCounter.h"

const int SCREEN_WIDTH = 128; // px
const int SCREEN_HEIGHT = 64; // px
const int OLED_RESET_PIN = 4; // Reset pin # (or -1 if sharing Arduino reset pin)

const int DELAY_MS = 200;

// https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/power-management
const float BATTERY_MIN = 3.2; // Volts
const float BATTERY_MAX = 4.2; // Volts

// Credit to accelerometer citation in Accelerometer.h
const int LIS3DH_CLK = 19; // SCL
const int LIS3DH_MISO = 17; // SDO
const int LIS3DH_MOSI = 16;  // SDA
const int LIS3DH_CS = 21; // CS

// Credit to step counter citation in StepCounter.h
const int MIN_DISTANCE_BETWEEN_PEAKS_MS = 300;
const int MIN_PEAK_HEIGHT = 15;

const int VOLTAGE_PIN = A13; // voltage needs to be multiplied by 2

const int SAMPLING_RATE = 50; // Hz
const int SAMPLING_INTERVAL = floor(1000/((float) SAMPLING_RATE)); // Ms

const int REFRESH_INTERVAL = 1000; // Ms

int _steps;
// Credit to accelerometer citation in Accelerometer.h
float _accelerationX;
float _accelerationY;
float _accelerationZ;
int _accelerationMagnitude;
long _accelerationTimestamp;
long _lastRefreshTimestamp;

// I have tried both keeping the accelerometer and oled in this class
// and factoring them out to their own classes.

Adafruit_LIS3DH _lis = Adafruit_LIS3DH();
// Adafruit_LIS3DH _lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// Accelerometer accelerometer = Accelerometer(LIS3DH_CLK, LIS3DH_MISO, LIS3DH_MOSI, LIS3DH_CS);
// Partial credit to OLED citation in Oled.h
// Oled oled = Oled(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);


// Partially derived from step calcuator citation in StepCalculator.h
StepCounter stepCounter = 
  StepCounter(MIN_DISTANCE_BETWEEN_PEAKS_MS, MIN_PEAK_HEIGHT, SAMPLING_RATE);

void setup(){
  Serial.begin(115200);
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(SCL, INPUT_PULLUP);
  pinMode(SDA, INPUT_PULLUP);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  accelerometerSetup();
  oledSetup();
  long timestamp = millis();
  _accelerationTimestamp = timestamp - SAMPLING_INTERVAL;
  _lastRefreshTimestamp = timestamp - REFRESH_INTERVAL;
}

void loop(){
  long timestamp = millis();
  if (timestamp >= _accelerationTimestamp + SAMPLING_INTERVAL) {
    updateAccelerometer();
    updateStepCounter();
    _accelerationTimestamp = timestamp;
  }
  if (timestamp >= _lastRefreshTimestamp + REFRESH_INTERVAL) {
    updateOled();
    _lastRefreshTimestamp = timestamp;
  }
}

void updateAccelerometer() {
  accelerometerUpdate();
  _accelerationTimestamp = millis();
}

void updateStepCounter() {
  stepCounter.update(_accelerationTimestamp, _lastRefreshTimestamp);
  _steps = stepCounter.getSteps();
}

void updateOled() {
  String message = "x: " + (String) _accelerationX
    + "\ny: " + (String) _accelerationY
    + "\nz: " + (String) _accelerationZ
    + "\nMagnitude: " + (String) _accelerationMagnitude
    + "\nSteps: " + (String) _steps
    + "\nBattery: " + (String) batteryPercentage() + "%";
  drawString(message);  
}

int batteryPercentage() {
  float totalVoltage = map(analogRead(VOLTAGE_PIN), 0, 4095, 0, 5) * 2;
  float percentage = (totalVoltage - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN) * 100;
  return (int) percentage;
}

// Accelerometer

void accelerometerSetup() {
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

void accelerometerUpdate() {
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

// Oled

void oledSetup() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
}

void drawString(String message1) {

  String message = "x: " + (String) _accelerationX
    + "\ny: " + (String) _accelerationY
    + "\nz: " + (String) _accelerationZ
    + "\nMagnitude: " + (String) _accelerationMagnitude
    + "\nSteps: " + (String) _steps
    + "\nBattery: " + (String) batteryPercentage() + "%";
  
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<message.length(); i++) {
    display.write(message[i]);
  }

  display.display();
}
