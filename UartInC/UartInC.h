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

    resetVariables();

    printVariables(brightness, "Brightness");
    printVariables(mode, "Mode");
    printVariables(r, "R");
    printVariables(g, "G");
    printVariables(b, "B");

    switch (mode) {
      case 1:
        emitStaticColourAll(r, g, b, 100);
        break;

      case 2:
        flashingLights(r, g, b, brightness);
        break;

      case 3:
        rainbowLights(brightness);
        break;

      case 4:
        fadingLights(r, g, b, brightness);
        break;

      case 5:
        patternLights(r, g, b, brightness);
        break;
    }
  }
}

#endif
