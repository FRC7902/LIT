#include <FastLED.h>
#include "Arduino.h"
#include "stdio.h"
#include "hardware/uart.h"

#define LED_PIN 2
#define NUM_LEDS 20
#define IRQ_PIN 3
CRGB leds[NUM_LEDS];

char r, g, b, mode, stringy[5];
bool irq_flag = false, direction = true, isToBeTurnedOn = true;
int i;

void emitStaticColourAll(int r, int g, int b, int brightness) {
  for (int i = 0; i != 20; i++) {
    leds[i] = CRGB(r, g, b);
    }
  FastLED.show(brightness);
}

void emitStaticColour(int r, int g, int b, int brightness, int ledIndex) {
  leds[ledIndex] = CRGB(r, g, b);
  FastLED.show(brightness);
}

void interrupt() {
  irq_flag = true;
}

void setup() {
  delay(2000);
  //Serial.begin(9600); //computer connection
  //delay(200);
  Serial1.begin(4800, SERIAL_8N1); //pico connection
  //Serial.println("SERIAL1_TX: " + String(SERIAL1_TX));
  //Serial.println("SERIAL1_RX: " + String(SERIAL1_RX));
  pinMode(IRQ_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), interrupt, RISING);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  emitStaticColourAll(0, 0, 0, 0);
}

void loop() {
  if (Serial1.available() >= 4) {
    irq_flag = false;
    mode = Serial1.read();
    r = Serial1.read();
    g = Serial1.read();
    b = Serial1.read();

    //sprintf(stringy, "%d", mode);
    //Serial.println(stringy);            

    if (mode == 1) {
      emitStaticColourAll(0, 0, 0, 0);
      emitStaticColourAll(r, g, b, 100);
    }

    else if (mode == 2) {
      emitStaticColourAll(0, 0, 0, 0);
      while (irq_flag == false) {
        emitStaticColourAll(r, g, b, 100);
        delay(500);
        emitStaticColourAll(0, 0, 0, 100);
        delay(500);
      }
    }

    else if (mode == 3) {
      i = 0;
      emitStaticColourAll(0, 0, 0, 0);
      while (irq_flag == false) {
        i = (direction == true) ? (i + 1) : (i - 1);
        emitStaticColourAll(i, 0, 255 - i, 100);
        delay(30);
        if (i == 255) {
          direction = false;
        }
        else if (i == 0) {
          direction = true;
        }
      }
    }

    else if (mode == 4) {
      i = 0;
      emitStaticColourAll(0, 0, 0, 0);
      while (irq_flag == false) {
        i = (direction == true) ? (i + 1) : (i - 1);
        emitStaticColourAll(r, g, b, i);
        delay(10);
        if (i == 255) {
          direction = false;
        }
        else if (i == 0) {
          direction = true;
        }
      }
    }

    else if (mode == 5) {
      i = 0;
      emitStaticColourAll(0, 0, 0, 0);      
      while (irq_flag == false) {
        if (isToBeTurnedOn == true) {
          emitStaticColour(r, g, b, 100, i);
        }
        else {
          emitStaticColour(0, 0, 0, 100, i);
        }
        delay(100);
        if (i == 20) {
          isToBeTurnedOn = !(isToBeTurnedOn);
          i = 0;
        }
        else {
          i++;
        }  
        

        }
      }
  }
}
