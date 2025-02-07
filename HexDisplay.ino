#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN 12
#define N_LEDS 147
#define MAX_BRIGHTNESS 255 // Choose a value between 2 and 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
// Only used for inital blackout operation
Adafruit_NeoPixel excess_strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void handleFire() {
  int heat[N_LEDS];
  int cooling = 55;
  int sparking = 120;
  long startTime = millis();

  while (millis() - startTime < 15000) {  // Run for 15 seconds
    // Step 1: Cool down every cell
    for (int i = 0; i < N_LEDS; i++) {
      heat[i] = (heat[i] - random(0, ((cooling * 10) / N_LEDS) + 2));
      if (heat[i] < 0) heat[i] = 0;  // Ensure heat doesn't go below 0
    }

    // Step 2: Heat from each cell drifts up and diffuses
    for (int i = N_LEDS - 1; i >= 2; i--) {
      heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    // Step 3: Randomly ignite new sparks near the bottom
    if (random(255) < sparking) {
      int y = random(7);
      heat[y] = heat[y] + random(160, 255);
    }

    // Step 4: Convert heat to LED colors with random flame colors
    for (int i = 0; i < N_LEDS; i++) {
      setPixelRandomColor(i, heat[i]);
    }

    strip.show();
    delay(50);
  }
}

void setPixelRandomColor(int pixel, int temperature) {
  int r = random(100, 256);
  int g = random(0, 100);
  int b = random(0, 100);
  int brightness = (temperature > 255) ? 255 : temperature;

  writePixelNoShow(pixel, r, g, b, brightness);
}

void writePixelNoShow(int n, int r, int g, int b, int brightness) {
  brightness = (brightness > MAX_BRIGHTNESS) ? MAX_BRIGHTNESS : brightness;

  // Apply brightness to RGB values directly using a more efficient bit shift
  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;

  strip.setPixelColor(n, r, g, b); // Draw new pixel
}

void lightUpAll() {
  for (int i = 0; i < N_LEDS; i++) {
      writePixelNoShow(i, 255, 0, 0, 255);
  }
}



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  handleFire();
}
