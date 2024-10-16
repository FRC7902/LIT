#ifndef COLOUR EFFECTS_H_
#define COLOUR EFFECTS_H_
#include "Functions.h"
#include "Constants.h"

void emitStaticColourAll(double r, double g, double b, double brightness)
{
  colour_set = urgb_u32((r), (g), (b));
  for (int x = 0; x < NUM_PIXELS; x++)
  {
    put_pixel(colour_set);
  }
}

void emitStaticColour(double r, double g, double b, double brightness, int ledIndex)
{
  if (direction == true)
  {
    for (int c = ledIndex; c >= 0; c--)
    {
      colour_set = (c == 0) ? (urgb_u32((r), (g), (b))) : (urgb_u32((0), (0), (0)));
      put_pixel(colour_set);
    }
  }
  else if (direction == false)
  {
    for (int c = 0; c <= ledIndex + 1; c++)
    {
      colour_set = (c == (ledIndex - 1)) ? (urgb_u32((r), (g), (b))) : (urgb_u32((0), (0), (0)));
      put_pixel(colour_set);
    }
  }
}

void turnOffAllLights()
{
  colour_set = urgb_u32((0), (0), (0));
  for (int x = 0; x < 60; x++)
  {
    put_pixel(colour_set);
  }
}

void flashingLights(int r, int g, int b, int brightness)
{
  while (irq_flag == false)
  {
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(flashingLightDelay);
    emitStaticColourAll(0, 0, 0, 0);
    sleep_ms(flashingLightDelay);
  }
}

void rainbowLights(int brightness) {
  i = 0.2;
  r = g = b = 255.0, 
  step = 1; 
  while (irq_flag == false) {
    i += 0.01;
    if (step == 1) {
      if (direction == true) {
        r-=rainbowLightConstant;
      }
      else if (direction == false) {
        r+=rainbowLightConstant;
      }
    }
    else if (step == 2) {
      if (direction == true) {
        r+=rainbowLightConstant;
        g-=rainbowLightConstant;
      }
      else if (direction == false) {
        r-=rainbowLightConstant;
        g+=rainbowLightConstant;
      }
    }
    else if (step == 3) {
      if (direction == true) {
        g+=rainbowLightConstant;
        b-=rainbowLightConstant;
      }
      else if (direction == false) {
        g-=rainbowLightConstant;
        b+=rainbowLightConstant;
      }
    }
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(rainbowLightDelay);
    if (i >= 14) {
      if (step == 3 && direction == true) {
        direction = false;
        step++;
      }
      else if (step == 1 && direction == false) {
        direction = true;
        step--;
      }
      if (direction == false) {
        step--;  
      }
      else if (direction == true) {
        step++;
      }
      i = 0.2; 
    }
  }
}

void patternLights(int r, int g, int b, int brightness)
{
  turnOffAllLights();
  while (irq_flag == false)
  {
    emitStaticColour(r, g, b, brightness, i);
    sleep_ms(patternLightDelay);
    if (i == NUM_PIXELS)
    {
      direction = false;
    }
    else if (i == 0 && direction == false)
    {
      direction = true;
    }
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
  while (irq_flag == false)
  {
    if (direction == true)
    {
      i++;
      r = decrementor(r);
      g = decrementor(g);
      b = decrementor(b);
    }
    else if (direction == false)
    {
      i--;
      r = incrementor(r);
      g = incrementor(g);
      b = incrementor(b);
    }
    emitStaticColourAll(r, g, b, i);
    sleep_ms(fadingLightDelay);
    if (i == 250)
    {
      direction = false;
    }
    else if (i == 1)
    {
      direction = true;
    }
  }
}

#endif