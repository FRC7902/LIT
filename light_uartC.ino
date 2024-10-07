#include <FastLED.h>
#include "Arduino.h"
#include "stdio.h"
#include "hardware/uart.h"

#define LED_PIN 2
#define NUM_LEDS 20
#define IRQ_PIN 3
CRGB leds[NUM_LEDS];

char mode, brightness, r, g, b, stringy[5];
bool irq_flag = false, direction = true, isToBeTurnedOn = true;
int i, bytes_transmitted;

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

void flashingLights(int r, int g, int b, int brightness) {
  emitStaticColourAll(0, 0, 0, 0);
  while (irq_flag == false) {
    emitStaticColourAll(r, g, b, brightness);
    delay(500);
    emitStaticColourAll(0, 0, 0, 0);
    delay(500);
  }
}

void rainbowLights(int brightness) {
  i = 0;
  emitStaticColourAll(0, 0, 0, 0);
  while (irq_flag == false) {
    i = (direction == true) ? (i + 1) : (i - 1);
    emitStaticColourAll(i, 0, 255 - i, brightness);
    delay(30);
    if (i == 253) {
      direction = false;
    }
    else if (i == 3) {
      direction = true;
    }
  }
}

void fadingLights(int r, int g, int b, int brightness) {
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

void patternLights(int r, int g, int b, int brightness) {
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

void interrupt() {
  irq_flag = true;
}

void setup() {
  delay(2200);
  //Serial.begin(9600); //computer connection
  //delay(200);
  Serial1.begin(300, SERIAL_8N1); //pico connection
  //Serial.println("SERIAL1_TX: " + String(SERIAL1_TX));
  //Serial.println("SERIAL1_RX: " + String(SERIAL1_RX));
  pinMode(IRQ_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), interrupt, RISING);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  emitStaticColourAll(0, 0, 0, 0);
}

void loop() {
  if (Serial1.available()) {
    brightness = mode = r = g = b = 0;
    bytes_transmitted = Serial1.available();
    //Serial.println(bytes_transmitted);
    irq_flag = false;
    brightness = (bytes_transmitted >= 0) ? (Serial1.read()) : (0);
    mode = (bytes_transmitted >= 1) ? (Serial1.read()) : (0);
    r = (bytes_transmitted >= 2) ? (Serial1.read()) : (0);
    g = (bytes_transmitted >= 3) ? (Serial1.read()) : (0);
    b = (bytes_transmitted >= 4) ? (Serial1.read()) : (0);

    delay(5);

    /*
    sprintf(stringy, "%d", brightness);
    Serial.println(stringy);    
    
    sprintf(stringy, "%d", mode);
    Serial.println(stringy);      

    sprintf(stringy, "%d", r);
    Serial.println(stringy);            

    sprintf(stringy, "%d", g);
    Serial.println(stringy);            

    sprintf(stringy, "%d", b);
    Serial.println(stringy);            
    */

    if (brightness == 0) {
      emitStaticColourAll(0, 0, 0, 0);
    }

    else if (mode == 1) {
      emitStaticColourAll(0, 0, 0, 0);
      emitStaticColourAll(r, g, b, brightness);
    }

    else if (mode == 2) {
      flashingLights(r, g, b, brightness);
    }

    else if (mode == 3) {
      rainbowLights(brightness);
    }

    else if (mode == 4) {
      fadingLights(r, g, b, brightness);
    }

    else if (mode == 5) {
      patternLights(r, g, b, brightness);
    }
  }
}
