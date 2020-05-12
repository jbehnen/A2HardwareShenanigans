#ifndef OLED_H
#define OLED_H

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

/**
* Updates the OLED display.
*   
* By Julia Behnen
* 
* Adapted from the demo code referenced at  https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples
*/ 
class Oled {
  private:
    const int _screenWidth;
    const int _screenHeight;
    const int _oledResetPin;
	
    Adafruit_SSD1306 _display;
    
  public:
    // Constructor
    Oled(int screenWidth, int screenHeight, int oledResetPin);
	
    void setup();
	
    void update(int steps);
	
    void drawString(String message);
};

#endif