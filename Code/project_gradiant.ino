#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4
#define TEMP_SENSOR_PIN 28

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const float BETA = 3950;

float readCelsius() {
    int analogValue = analogRead(TEMP_SENSOR_PIN);
    return 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    strip.begin();
    strip.show();
}

int scaledValue(int val, int v1min, int v1max, int v2min, int v2max) {
    int v1range = v1max - v1min;
    int v2range = v2max - v2min;
    int ratio = v2range / v1range;
    return (val - v1min) * ratio + v2min;
}

void loop() {
    int currTemp = (int)readCelsius();
    int rgbScaled;
    if (currTemp < -20) {
        rgbScaled = 0;
    } else if (currTemp > 20) {
        rgbScaled = 255;
    } else {
        rgbScaled = scaledValue(currTemp, -20, 20, 0, 255);
    }
    for (int currPx = 0; currPx < PIXEL_COUNT; currPx++) {
        strip.setPixelColor(currPx, rgbScaled, 0, 255-rgbScaled);
    }
    strip.show();
    delay(20);
}
