#include "pico/stdlib.h"
#include "ws2812/ws2812.pio.h"

#define UART_ID uart0
#define BAUD_RATE 300
#define UART_RX_PIN 1
#define WS2812_PIN 2
#define IRQ_PIN 3

int mode, brightness, NUM_PIXELS = 20, step, stringy[5];
bool irq_flag = false, direction = true;
float i, constant, r, g, b;
uint32_t colour_set;

static inline void put_pixel(uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         (uint32_t)(b);
}

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
    sleep_ms(500);
    emitStaticColourAll(0, 0, 0, 0);
    sleep_ms(500);
  }
}

void rainbowLights(int brightness) {
  constant = 0.3,
  i = 0.2;
  r = g = b = 255.0, 
  step = 1; 
  while (irq_flag == false) {
    i += 0.01;
    if (step == 1) {
      if (direction == true) {
        r-=constant;
      }
      else if (direction == false) {
        r+=constant;
      }
    }
    else if (step == 2) {
      if (direction == true) {
        r+=constant;
        g-=constant;
      }
      else if (direction == false) {
        r-=constant;
        g+=constant;
      }
    }
    else if (step == 3) {
      if (direction == true) {
        g+=constant;
        b-=constant;
      }
      else if (direction == false) {
        g-=constant;
        b+=constant;
      }
    }
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(3);
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

void fadingLights(int r, int g, int b, int brightness)
{
  while (irq_flag == false)
  {
    if (direction == true)
    {
      i++;
      r = (r == 0) ? (r) : (r - 1);
      g = (g == 0) ? (g) : (g - 1);
      b = (b == 0) ? (b) : (b - 1);
    }
    else if (direction == false)
    {
      i--;
      r = (r == 0) ? (r) : (r + 1);
      g = (g == 0) ? (g) : (g + 1);
      b = (b == 0) ? (b) : (b + 1);
    }
    emitStaticColourAll(r, g, b, i);
    sleep_ms(2);
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

void patternLights(int r, int g, int b, int brightness)
{
  turnOffAllLights();
  while (irq_flag == false)
  {
    emitStaticColour(r, g, b, brightness, i);
    sleep_ms(50);
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

void interrupt()
{
  irq_flag = true;
}

int main()
{
  stdio_init_all();
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
  gpio_set_dir(IRQ_PIN, GPIO_OUT);
  gpio_pull_down(IRQ_PIN);
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_RISE, true, interrupt);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  while (true)
  {
    if (uart_is_readable(UART_ID))
    {
      irq_flag = false, direction = true;
      brightness = mode = r = g = b = i = 0;
      brightness = uart_getc(UART_ID);
      mode = uart_getc(UART_ID);
      r = uart_getc(UART_ID);
      g = uart_getc(UART_ID);
      b = uart_getc(UART_ID);

      /*
      brightness = (uart_is_readable(UART_ID)) ? (uart_getc(UART_ID)) : (0);
      mode = (uart_is_readable(UART_ID)) ? (uart_getc(UART_ID)) : (0);
      r = (uart_is_readable(UART_ID)) ? (uart_getc(UART_ID)) : (0);
      g = (uart_is_readable(UART_ID)) ? (uart_getc(UART_ID)) : (0);
      b = (uart_is_readable(UART_ID)) ? (uart_getc(UART_ID)) : (0);
      */

      sprintf(stringy, "%d", brightness);
      printf(stringy);
      printf("\nBrightness:\n\n");

      sprintf(stringy, "%d", mode);
      printf(stringy);
      printf("\nMode:\n\n");

      sprintf(stringy, "%d", r);
      printf(stringy);
      printf("\nR:\n\n");

      sprintf(stringy, "%d", g);
      printf(stringy);
      printf("\nG:\n\n");

      sprintf(stringy, "%d", b);
      printf(stringy);
      printf("\nB:\n\n");

      if (brightness == 0) {
        if (mode == 5) {
          turnOffAllLights();
        }
        else {
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
}
