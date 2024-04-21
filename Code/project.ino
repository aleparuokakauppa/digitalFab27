#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_CONTROL_PIN 9
#define ACTIVE_PIXEL_COUNT 4
#define MAX_PIXEL_COUNT 60
#define TEMP_SENSOR_PIN 28
#define BUTTON_PIN 27

/**
 *MAX_ADC 4095
 *MIN_ADC 0
 *MAX_SENSOR_TEMP 125
 *MIN_SENSOR_TEMP -40
 *cToDigRatio = (MAX_ADC - MIN_ADC) / (MAX_SENSOR_TEMP - MIN_SENSOR_TEMP)
 */
static const double cToDigRatio = 24.81;

struct rgbColor {
    uint8_t red,
    uint8_t green,
    uint8_t blue
};

static rgbColor currColor = {0, 0, 0};
static const int ledInterval = 10; // Distance between lit LEDs
static bool ledState = true;

Adafruit_NeoPixel strip(MAX_PIXEL_COUNT, NEOPIXEL_CONTROL_PIN, NEO_GRB + NEO_KHZ800);

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
 *Sets the pixels for the strip according to if the ledState is true or not
 */
void setPixelsUseState(rgbColor newColor) {
    if (ledState == false) {
        strip.clear();
        return;
    }
    for (int i = 0; i < PIXEL_COUNT; i++) {
        int currPx = i * ledInterval;
        if (currPx <= MAX_PIXEL_COUNT) {
            strip.setPixelColor(currPx, newColor.red, newColor.green, newColor.blue);
        }
    }
}

void buttonInterrupt() {
    // ledState flip flop
    ledState = (ledState == true) ? false : true;
}

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(NEOPIXEL_CONTROL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, LOW);
    strip.begin();
    strip.show();
}

void loop() {
    float currTemp = readTemperature();
    Serial.println(currTemp);

    // For color scaling
    const int minTemp = 0;
    const int maxTemp = 20;

    int rgbScaled;
    if (currTemp < minTemp) {
        rgbScaled = 0;
    } else if (currTemp > maxTemp) {
        rgbScaled = 255;
    } else {
        rgbScaled = scaledValue(static_cast<int>(currTemp), minTemp, maxTemp, 0, 255);
    }

    currColor = {
        scaledValue(rgbScaled, 0, 255, 100, 255);
        0,
        255-rgbScaled
    };

    setPixelsUseState(currColor);
    strip.show();

    // Update rate for temperature change
    delay(1000);
}

