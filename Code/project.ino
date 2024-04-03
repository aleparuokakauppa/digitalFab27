#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4
#define TEMP_SENSOR_PIN 28

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const float BETA = 3950;

void setup() {
  strip.begin();
  strip.show();
}

float readCelsius() {
  int analogValue = analogRead(TEMP_SENSOR_PIN);
  return 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void fillStrip(int color) {
  uint32_t currColor;
  switch (color) {
  case 0:
    currColor = strip.Color(0, 0, 255);
    break;
  case 1:
    currColor = strip.Color(0, 255, 0);
    break;
  case 2:
    currColor = strip.Color(255, 0, 0);
    break;
  default:
    currColor = strip.Color(100, 100, 100);
  }
  for (int currPx = 0; currPx < PIXEL_COUNT; currPx++) {
    strip.setPixelColor(currPx, currColor);
  }
  strip.show();
}

void loop() {
  float currTemp = readCelsius();
  if (currTemp < 0) {
    fillStrip(0);
  } else if (currTemp < 10) {
    fillStrip(1);
  } else if (currTemp < 15) {
    fillStrip(2);
  }
  delay(2000);
}
