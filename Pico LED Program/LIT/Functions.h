#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "Constants.h"
#include "LIT.h"
#include "ws2812.pio.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "C:\Users\spara\.pico-sdk\sdk\2.0.0\src\rp2_common\pico_sleep\include\pico\sleep.h"

void startProgram();


static inline void put_pixel(uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}


void enter_sleep()
{
  sleep_run_from_xosc();
  sleep_goto_dormant_until_pin(IRQ_PIN, false, true);
  gpio_put(25, 1);
  startProgram();
}


static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         (uint32_t)(b);
}


int incrementOrDecrement(int input, bool type, bool isI)
{
  if (input == 0 && isI == false)
  {
    return input;
  }
  else if (type == false)
  {
    i--;
    return input + 1;
  }
  else if (type == true)
  {
    i++;
    return input - 1;
  }
}


float getInput(int type)
{
  if (type == 0)
  {
    return ((float)(uart_getc(UART_ID)));
  }
  else if (type == 1)
  {
    return 127 + ((float)(uart_getc(UART_ID)));
  }
}

void resetVariables()
{
  i = 0;
  direction = true;
  rainbowLightConstant = 0.3;
  brightness = getInput(0);
  mode = getInput(0);
  r = getInput(1);
  g = getInput(1);
  b = getInput(1);
}

void printVariables(float x, char name[])
{
  printf("%.1f", x);
  printf("\n", name, "\n\n", "%s");
}

void setup()
{
  stdio_init_all();
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
  uart_set_format(UART_ID, 8, 2, UART_PARITY_NONE);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_init(UART_ID, BAUD_RATE);
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_pull_down(IRQ_PIN);
  gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_RISE, true, startProgram);
  uart_set_fifo_enabled(UART_ID, false);
}

#endif
