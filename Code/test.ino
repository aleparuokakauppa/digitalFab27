#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 8
#define PIXEL_COUNT 4

Adafruit_NeoPixel strip(PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

void setup() {
    Serial.begin(9600);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    for (int i = 0; i < PIXEL_COUNT; i++) {
        Serial.print("Set pixel ");
        Serial.print(i);
        Serial.println(" color");
        strip.setPixelColor(i, 255, 0, 0);
    }
    strip.begin();
    strip.show();
}

void loop() {
    delay(1000);
}
