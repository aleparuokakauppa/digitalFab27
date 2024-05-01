#include <Adafruit_NeoPixel.h>
#include <hardware/adc.h>
#include <queue>

#define NEOPIXEL_CONTROL_PIN 9
#define ACTIVE_PIXEL_COUNT 4
#define MAX_PIXEL_COUNT 10
#define TEMP_SENSOR_PIN 28

static const int minTemp = 10;
static const int maxTemp = 40;

static const int avgTempSamples = 15;
std::queue<float> temps;

struct rgbColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

static rgbColor currColor = {0, 0, 0};
static const int ledInterval = 3; // Distance between lit LEDs

Adafruit_NeoPixel strip(MAX_PIXEL_COUNT, NEOPIXEL_CONTROL_PIN, NEO_GRB + NEO_KHZ800);

float getQueueAvg(std::queue<float> q) {
    if (q.empty())
      return 0.0;
    
    float sum = 0.0;
    int count = q.size();

    std::queue<float> tempQueue = q;
    while (!tempQueue.empty()) {
        sum += tempQueue.front();
        tempQueue.pop();
    }

    Serial.print("Queue sum: ");
    Serial.println(sum);
    Serial.print("Queue count: ");
    Serial.println(count);

    return sum / count;
}

float readAvgTemperature() {
    float currTemp = readTemperature();
    Serial.print("Raw currTemp: ");
    Serial.println(currTemp);
    if (temps.size() >= avgTempSamples)
        temps.pop();
    temps.push(currTemp);
    return getQueueAvg(temps);
}

float readTemperature() {
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    Serial.println(result);
    double voltage = result * conversion_factor;
    Serial.print("Voltage: ");
    Serial.println(voltage);
    return (voltage - 0.5) * 100;
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
    adc_init();
    adc_gpio_init(TEMP_SENSOR_PIN);
    adc_select_input(2);
    pinMode(NEOPIXEL_CONTROL_PIN, OUTPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    strip.begin();
    strip.show();
}

void setColor(float temp) {
    int r, g, b;
    float normalizedValue = (temp - minTemp) / (maxTemp - minTemp);
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
    while (temps.size() < avgTempSamples) {
        float currTemp = readAvgTemperature();
    }
    float currTemp = readAvgTemperature();
    Serial.print("Average Temp: ");
    Serial.println(currTemp);

    currTemp = (currTemp < minTemp) ? minTemp : currTemp;
    currTemp = (currTemp > maxTemp) ? maxTemp : currTemp;

    setColor(currTemp);
    setPixels(currColor);
    Serial.print("Color: (");
    Serial.print(currColor.red);
    Serial.print(", ");
    Serial.print(currColor.green);
    Serial.print(", ");
    Serial.print(currColor.blue);
    Serial.println(")");
    Serial.println();
    strip.show();

    // Update rate for temperature change
    delay(2000);
}
