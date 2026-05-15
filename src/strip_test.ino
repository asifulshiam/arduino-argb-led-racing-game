#include <Adafruit_NeoPixel.h>

#define LED_PIN 6         // Pin connected to the LED strip
#define NUM_LEDS 60       // Total number of LEDs in your strip
#define BUTTON_UP 7       // Pin for the "up" button

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentLED = 0;       // Start position of the LEDs
int lastButtonUpState = HIGH;

void setup() {
  strip.begin();
  strip.setBrightness(50);  // Adjust brightness for reduced power consumption
  strip.show();             // Ensure all LEDs are off

  pinMode(BUTTON_UP, INPUT_PULLUP);  // Set button pin as input with pullup
}

void loop() {
  int buttonUpState = digitalRead(BUTTON_UP);

  // Move LEDs up if button is pressed
  if (buttonUpState == LOW && lastButtonUpState == HIGH) {
    currentLED++;
    if (currentLED >= NUM_LEDS) currentLED = 0; // Wrap around to the first LED
    updateRainbowLEDs();
  }

  lastButtonUpState = buttonUpState;

  delay(100); // Faster movement
}

void updateRainbowLEDs() {
  strip.clear();  // Turn off all LEDs

  // Light up 4 LEDs at a time with rainbow colors
  for (int i = 0; i < 4; i++) {
    int ledIndex = (currentLED + i) % NUM_LEDS;  // Wrap around the strip
    strip.setPixelColor(ledIndex, Wheel((ledIndex * 256 / NUM_LEDS) & 255));
  }

  strip.show();
}

// Generate rainbow colors across 0-255 positions
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
