#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

void setup() {
    pinMode(NEOPIXEL_PIN, OUTPUT);
    strip.begin();
    strip.show();
}

void loop() {
    for (int i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, 255, 0, 0);
        strip.show();
        delay(1000);
    }
    for (int i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, 0, 255, 0);
        strip.show();
        delay(1000);
    }
    for (int i = 0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, 0, 0, 255);
        strip.show();
        delay(1000);
    }
    delay(5000);
}
