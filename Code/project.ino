#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_CONTROL_PIN 9
#define ACTIVE_PIXEL_COUNT 4
#define MAX_PIXEL_COUNT 60
#define TEMP_SENSOR_PIN 28

static const int minTemp = 0;
static const int maxTemp = 20;

struct rgbColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

static rgbColor currColor = {0, 0, 0};
static const int ledInterval = 10; // Distance between lit LEDs

Adafruit_NeoPixel strip(MAX_PIXEL_COUNT, NEOPIXEL_CONTROL_PIN, NEO_GRB + NEO_KHZ800);

float readTemperature() {
    int reading = analogRead(TEMP_SENSOR_PIN);
    double voltage = (reading * 3.3) / 4095.0;
    return voltage * 100;
}

void setPixels(rgbColor newColor) {
    for (int i = 0; i < ACTIVE_PIXEL_COUNT; i++) {
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
    float normalizedValue = (value - minTemp) / (maxTemp - minTemp);
    if (normalizedValue <= 0.5f) {
        r = 0;
        g = static_cast<int>(255 * (2 * normalizedValue));
        b = static_cast<int>(255 * (1 - 2 * normalizedValue));
    } else {
        r = static_cast<int>(255 * (2 * normalizedValue - 1));
        g = static_cast<int>(255 * (2 - 2 * normalizedValue));
        b = 0;
    }

    currColor = {r,g,b};
}

void loop() {
    float currTemp = readTemperature();
    Serial.println(currTemp);

    currTemp = (currTemp < minTemp) ? minTemp : currTemp;
    currTemp = (currTemp > maxTemp) ? maxTemp : currTemp;

    setColor(currTemp);
    setPixels(currColor);
    strip.show();

    // Update rate for temperature change
    delay(500);
}

