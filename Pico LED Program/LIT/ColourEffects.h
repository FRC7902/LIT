#ifndef COLOUREFFECTS_H_
#define COLOUREFFECTS_H_
#include "Functions.h"
#include <stdlib.h>

void emitStaticColourAll(double r, double g, double b, double brightness)
{
  for (int x = 0; x < NUM_PIXELS; x++) {
    put_pixel(urgb_u32((r), (g), (b)));
  }
}

void emitStaticColour(int r, int g, int b, int brightness, int ledIndex)
{
  for (; ledIndex >= 0 && direction == true; ledIndex--)
  {
    put_pixel((ledIndex == 0) ? (urgb_u32((r), (g), (b))) : (urgb_u32((0), (0), (0))));
  }
  for (ledIndex = 0; ledIndex <= i + 1 && direction == false; ledIndex++)
  {
    put_pixel((ledIndex == (i - 1)) ? (urgb_u32((r), (g), (b))) : (urgb_u32((0), (0), (0))));
  }
}

void turnOffAllLights()
{
  for (i = 0; i < 60; i++)
  {
    put_pixel(urgb_u32((0), (0), (0)));
  }
}

void flashingLights(int r, int g, int b, int brightness)
{
  while (true)
  {
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(flashingLightDelay);
    emitStaticColourAll(0, 0, 0, 0);
    sleep_ms(flashingLightDelay);
  }
}

void rainbowLights(int brightness)
{
  r = g = b = 255.0;
  while (true)
  {
    rainbowLightConstant = (direction == false) ? (rainbowLightConstant * -1) : abs(rainbowLightConstant);
    for (i = 0.2; i < abs(rainbowLightComparison); i += 0.01)
    {
      r -= rainbowLightConstant;
      emitStaticColourAll(r, g, b, 100);
      sleep_ms(rainbowLightDelay);
    }
    for (i = 0.2; i < abs(rainbowLightComparison); i += 0.01)
    {
      g -= rainbowLightConstant;
      emitStaticColourAll(r, g, b, 100);
      sleep_ms(rainbowLightDelay);
    }
    for (i = 0.2; i < abs(rainbowLightComparison); i += 0.01)
    {
      b -= rainbowLightConstant;
      emitStaticColourAll(r, g, b, 100);
      sleep_ms(rainbowLightDelay);
    }
    direction = !(direction);
  }
}

void patternLights(int r, int g, int b, int brightness)
{
  turnOffAllLights();
  while (true)
  {
    emitStaticColour(r, g, b, brightness, i);
    sleep_ms(patternLightDelay);
    direction = (i != NUM_PIXELS && direction == true) || (i == 0 && direction == false);
    if (direction == false)
    {
      i--;
    }
    else if (direction == true)
    {
      i++;
    }
  }
}

void fadingLights(int r, int g, int b, int brightness)
{
  while (true)
  {
    r = incrementOrDecrement(r, direction, false);
    g = incrementOrDecrement(g, direction, false);
    b = incrementOrDecrement(b, direction, false);
    i = incrementOrDecrement(i, !(direction), true);
    direction = (i != 250 && direction == true) || (i == 1 && direction == false);
    emitStaticColourAll(r, g, b, i);
    sleep_ms(fadingLightDelay);
  }
}

#endif