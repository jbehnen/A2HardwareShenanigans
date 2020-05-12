#include "Oled.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

Oled :: Oled(int screenWidth, int screenHeight, int oledResetPin) : 
  _screenWidth(screenWidth), _screenHeight(screenHeight), _oledResetPin(oledResetPin) {
  _display = Adafruit_SSD1306(screenWidth, screenHeight, &Wire, oledResetPin);
}

void Oled :: setup() {
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  _display.display();
}

void Oled :: update(int steps) {}

void Oled :: drawString(String message) {
  _display.clearDisplay();

  _display.setTextSize(1);      // Normal 1:1 pixel scale
  _display.setTextColor(SSD1306_WHITE); // Draw white text
  _display.setCursor(0, 0);     // Start at top-left corner
  _display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<message.length(); i++) {
    _display.write(message[i]);
  }

  _display.display();
}
