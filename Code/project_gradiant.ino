#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4
#define TEMP_SENSOR_PIN 28 // Should be a pin that supports ADC!
#define MAX_ADC 4095
#define MIN_ADC 0
#define MAX_SENSOR_TEMP 125
#define MIN_SENSOR_TEMP -40

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int scaledValue(int val, int v1min, int v1max, int v2min, int v2max) {
    int v1range = v1max - v1min;
    int v2range = v2max - v2min;
    int ratio = v2range / v1range;
    return (val - v1min) * ratio + v2min;
}

const double cToDigRatio = 24.81
void setup() {
    // Can be defined as 397.18182 at start
    // ratio = (MAX_ADC - MIN_ADC) / (MAX_SENSOR_TEMP - MIN_SENSOR_TEMP)
    // (65335 - 0) / (125 - (-40)) = 397.181818181818182
    Serial.begin(9600);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    strip.begin();
    strip.show();
}

int readTemperature() {
    return static_cast<int>(analogRead(TEMP_SENSOR_PIN) * cToDigRatio);
}

void loop() {
    int currTemp = readTemperature();
    const int minTemp = -20;
    const int maxTemp = 20;
    int rgbScaled;
    if (currTemp < minTemp) {
        rgbScaled = 0;
    } else if (currTemp > maxTemp) {
        rgbScaled = 255;
    } else {
        rgbScaled = scaledValue(currTemp, minTemp, maxTemp, 0, 255);
    }
    for (int currPx = 0; currPx < PIXEL_COUNT; currPx++) {
        strip.setPixelColor(currPx, rgbScaled, 0, 255-rgbScaled);
    }
    strip.show();
    // Doesn't need to run as fast as possible
    delay(1000);
}
