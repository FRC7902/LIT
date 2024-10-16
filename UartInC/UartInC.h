#ifndef UARTINC_H_
#define UARTINC_H_
#include "pico/stdlib.h"
#include "ws2812/ws2812.pio.h"
#include "Constants.h"
#include "Functions.h"
#include "Colour Effects.h"

void startProgram()
{
  if (uart_is_readable(UART_ID))
  {
    irq_flag = false, direction = true;
    brightness = getInput(2);
    mode = getInput(1);
    r = getInput(2);
    g = getInput(2);
    b = getInput(2);

    printVariables(brightness, "Brightness");
    printVariables(mode, "Mode");
    printVariables(r, "R");
    printVariables(g, "G");
    printVariables(b, "B");

    if (brightness == 0)
    {
      if (mode == 5)
      {
        turnOffAllLights();
      }
      else
      {
        emitStaticColourAll(0, 0, 0, 0);
      }
    }

    else if (mode == 1)
    {
      emitStaticColourAll(r, g, b, 100);
    }

    else if (mode == 2)
    {
      flashingLights(r, g, b, brightness);
    }

    else if (mode == 3)
    {
      rainbowLights(brightness);
    }

    else if (mode == 4)
    {
      fadingLights(r, g, b, brightness);
    }

    else if (mode == 5)
    {
      patternLights(r, g, b, brightness);
    }
  }
}

#endif
