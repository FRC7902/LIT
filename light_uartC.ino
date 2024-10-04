#include <FastLED.h>
#include "Arduino.h"
#include "stdio.h"
#include "hardware/uart.h"

#define LED_PIN 2
#define NUM_LEDS 20


CRGB leds[NUM_LEDS];
int rgb[] = {0, 1, 2, 3, 4, 5, 6};
          //First 0 = Pattern

void emitStaticColour(int r, int g, int b) {
  for (int i = 0; i != 20; i++) {
    leds[i] = CRGB(r, g, b);
    }
  FastLED.show();
}


void flashingLights(int r, int g, int b) {
  emitStaticColour(r, g, b);
  delay(500);
  emitStaticColour(0, 0, 0);
  delay(500);
}

void emitAlternatingColour(int r, int g, int b, int r2, int g2, int b2) {
  emitStaticColour(r, g, b);
  delay(1000);
  emitStaticColour(r2, g2, b2);
  delay(1000);
}


void setup() {
  delay(2000);
  Serial.begin(4800);
  delay(200);
  Serial1.begin(4800, SERIAL_8N1);  //actually, it's default
  Serial.println("SERIAL1_TX: " + String(SERIAL1_TX));
  Serial.println("SERIAL1_RX: " + String(SERIAL1_RX));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  emitStaticColour(0, 0, 0);
}

void loop() {
  for (int i = 0; Serial.available(); i++) {
    delay(2);
    rgb[i] = Serial.read();
  }

  if (rgb[0] == -1) {
    emitStaticColour(0, 0, 0);
    exit(0);
  }

  else if (rgb[0] == 1) {
    while (true) {
      flashingLights(rgb[1], rgb[2], rgb[3]);
      }
    }

  else if (rgb[0] == 2) {
    while (true) {
      emitAlternatingColour(rgb[1], rgb[2], rgb[3], rgb[4], rgb[5], rgb[6]);
    }
  }  
}


