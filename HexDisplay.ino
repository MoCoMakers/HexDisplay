#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <math.h>
#include "StateMachine.h"
#include "font.h"

#define PIN 12
#define N_LEDS 145
#define MAX_BRIGHTNESS 100 // Choose a value between 2 and 255

#define MATRIX_WIDTH 25
#define MATRIX_HEIGHT 5


void drawChar(int column, const uint8_t bitmap[5]) {
  for (int row = 0; row < 5; ++row) {
    for (uint8_t b = 0; b < 5; ++b) {
      bool lit = (bitmap[row]>>b) & 0b1;
      if (lit) {
        setPixel(column + 4 - b, row, 255, 0, 0, 100);
      } else {
        // If there is a solid background behind
        //setPixel(column + 4 - b, row, 0, 255, 0, 100);
      }
    }
  }
}
int start_index_row_0 = 0;
int start_index_row_1 = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
// Only used for inital blackout operation
Adafruit_NeoPixel excess_strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

StateMachine animationion1(1000, true);
StateMachine animationion2(350, true);
StateMachine backgroundAnimation(100, true);

void setPixel(int x, int y, int r, int g, int b, int brightness) {
  if (x < 0 || y < 0 || x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT) { return; }
  int location = N_LEDS - y*30 - x - 1;
  writePixelNoShow(location, r, g, b, brightness); 
}

void drawStripes(int offset) {
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      int r = 0;
      int g = 0;
      int b = 0;
      switch ((i+offset) % 3) {
        case 0:
          r = 255;
          break;
        case 1:
          b = 255;
          break;
        case 2:
          g = 255;
          break;
      }
      setPixel(i,j,r,g,b,100);
    }
  }
  strip.show();
  delay(50);
}

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

static uint8_t animation_state = 0;
static int column_start = 0;

void setStaticBackground() {
  strip.fill((0,0, 255), 0, N_LEDS);
}

void makeSolidBackground() {
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      int r = 0;
      int g = 0;
      int b = 255;
      setPixel(i,j,r,g,b,100);
    }
  }
}

void drawBg(){
  makeSolidBackground();
}

void setup() {
  // put your setup code here, to run once:
  

}

void loop() {
  if (animationion2.update()) {
    column_start--;

    if (column_start<-60) {
      column_start = 25;
    }
  }

  if (backgroundAnimation.update()) {
    // TODO change bg params
  }

  strip.clear();
  drawBg();
  drawChar(column_start+0, bitmap_M);
  drawChar(column_start+6, bitmap_O);
  drawChar(column_start+12, bitmap_C);
  drawChar(column_start+18, bitmap_O);
  drawChar(column_start+24, bitmap_M);
  drawChar(column_start+30, bitmap_A);
  drawChar(column_start+36, bitmap_K);
  drawChar(column_start+42, bitmap_E);
  drawChar(column_start+48, bitmap_R);
  drawChar(column_start+54, bitmap_S);
  strip.show();
  delay(50);
}
