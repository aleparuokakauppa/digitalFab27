#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_CONTROL_PIN 9
#define ACTIVE_PIXEL_COUNT 4
#define MAX_PIXEL_COUNT 60
#define TEMP_SENSOR_PIN 28

/**
 *MAX_ADC 4095
 *MIN_ADC 0
 *MAX_SENSOR_TEMP 125
 *MIN_SENSOR_TEMP -40
 *cToDigRatio = (MAX_ADC - MIN_ADC) / (MAX_SENSOR_TEMP - MIN_SENSOR_TEMP)
 */
static const double cToDigRatio = 24.81;

struct rgbColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

static rgbColor currColor = {0, 0, 0};
static const int ledInterval = 10; // Distance between lit LEDs

Adafruit_NeoPixel strip(MAX_PIXEL_COUNT, NEOPIXEL_CONTROL_PIN, NEO_GRB + NEO_KHZ800);

float readTemperature() {
    return analogRead(TEMP_SENSOR_PIN) / cToDigRatio;
}

void setPixels(rgbColor newColor) {
    for (int i = 0; i < PIXEL_COUNT; i++) {
        int currPx = i * ledInterval;
        if (currPx <= MAX_PIXEL_COUNT) {
            strip.setPixelColor(currPx, newColor.red, newColor.green, newColor.blue);
        }
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(NEOPIXEL_CONTROL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    strip.begin();
    strip.show();
}

void setColor(float temp) {
    int r, g, b;
    if (temp <= 10) {
        r = static_cast<int>(255 * (1 - temp / 10.0f));
        g = static_cast<int>(255 * (temp / 10.0f));
        b = 0;
    } else {
        r = 0;
        g = static_cast<int>(255 * ((20 - temp) / 10.0f));
        b = static_cast<int>(255 * ((temp - 10) / 10.0f));
    }

    currColor = {r,g,b};
}

void loop() {
    float currTemp = readTemperature();
    Serial.println(currTemp);

    const int minTemp = 0;
    const int maxTemp = 20;

    currTemp = (currTemp < minTemp) ? minTemp : currTemp;
    currTemp = (currTemp > maxTemp) ? maxTemp : currTemp;

    setColor(currTemp);
    setPixels(currColor);
    strip.show();

    // Update rate for temperature change
    delay(500);
}

