#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4
#define TEMP_SENSOR_PIN 28

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

int scaledValue(int val, int v1min, int v1max, int v2min, int v2max) {
    int v1range = v1max - v1min;
    int v2range = v2max - v2min;
    int ratio = v2range / v1range;
    return (val - v1min) * ratio + v2min;
}

/**
 *MAX_ADC 4095
 *MIN_ADC 0
 *MAX_SENSOR_TEMP 125
 *MIN_SENSOR_TEMP -40
 */
// (MAX_ADC - MIN_ADC) / (MAX_SENSOR_TEMP - MIN_SENSOR_TEMP) =
const double cToDigRatio = 24.81

void setup() {
    Serial.begin(9600);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    strip.begin();
    strip.show();
}

int readTemperature() {
    return static_cast<int>(analogRead(TEMP_SENSOR_PIN) / cToDigRatio);
}

void loop() {
    int currTemp = readTemperature();
    Serial.print("Read temperature as celsius: ");
    Serial.println(currTemp);
    const int minTemp = -30;
    const int maxTemp = 30;
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
