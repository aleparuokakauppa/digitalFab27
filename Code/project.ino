#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 9
#define PIXEL_COUNT 4
#define TEMP_SENSOR_PIN 28
#define DIN_PIN 27

/**
 *MAX_ADC 4095
 *MIN_ADC 0
 *MAX_SENSOR_TEMP 125
 *MIN_SENSOR_TEMP -40
 */
// (MAX_ADC - MIN_ADC) / (MAX_SENSOR_TEMP - MIN_SENSOR_TEMP) =
static const double cToDigRatio = 24.81;
static bool ledState = true;
static int buttonVal;

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int scaledValue(int val, int v1min, int v1max, int v2min, int v2max) {
  int v1range = v1max - v1min;
  int v2range = v2max - v2min;
  int ratio = v2range / v1range;
  return (val - v1min) * ratio + v2min;
}

float readTemperature() {
  return analogRead(TEMP_SENSOR_PIN) / cToDigRatio;
}

/*
  Sets the pixels for the strip according to if the ledState is true or not
*/
void setPixelsUseState(uint8_t red, uint8_t green, uint8_t blue) {
  if (ledState == true) {
    for (int currPx = 0; currPx < PIXEL_COUNT; currPx++) {
      strip.setPixelColor(currPx, red, green, blue);
    }
  } else {
    strip.clear();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(DIN_PIN, INPUT_PULLUP);
  pinMode(NEOPIXEL_PIN, OUTPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  strip.begin();
  strip.show();
}

void loop() {
    // Update rate for the entire loop
    delay(200);

    float currTemp = readTemperature();
    Serial.print("Read temperature as celsius: ");
    Serial.println(currTemp);
    const int minTemp = 6;
    const int maxTemp = 14;
    int rgbScaled;
    if (currTemp < minTemp) {
      rgbScaled = 0;
    } else if (currTemp > maxTemp) {
      rgbScaled = 255;
    } else {
      rgbScaled = scaledValue(static_cast<int>(currTemp), minTemp, maxTemp, 0, 255);
    }

    buttonVal = digitalRead(DIN_PIN);
    if (buttonVal == 0) {
      ledState = (ledState == true) ? false : true;
      setPixelsUseState(rgbScaled, 255-rgbScaled, 0);
      strip.show();
      // Hang until button is unpressed
      while (buttonVal == 0) {
        buttonVal = digitalRead(DIN_PIN);
      }
    }

    setPixelsUseState(rgbScaled, 255-rgbScaled, 0);
    strip.show();
}

