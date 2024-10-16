#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "Constants.h"
#include "UartInC.h"

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

int incrementor(int input) {
  return (input == 0) ? (input) : (input + 1);
}

int decrementor(int input) {
  return (input == 0) ? (input) : (input - 1);
}

float getInput(int variableType) {
  int type1;
  float type2;
  
  if (variableType == 1) {
    type1 = (int)(uart_getc(UART_ID));
    return (type1 < 0) ? (abs(type1) - type1) : (type1);
  }
  else if (variableType == 2) {
    type2 = (float)(uart_getc(UART_ID));
    return (type2 < 0) ? (abs(type2) - type2) : (type2);
  }
}

void resetVariables() {
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
}

void printVariables(float x, char name[]) {
  sprintf(stringy, "%f", x);
  printf(stringy);
  printf("\n");
  printf(name);
  printf(":\n\n");
}

void interrupt()
{
  irq_flag = true;
  printf("interrupt received");
  startProgram();
}

void setup() {
  stdio_init_all();
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
  gpio_set_dir(IRQ_PIN, GPIO_OUT);
  gpio_pull_down(IRQ_PIN);
  uart_set_format(UART_ID, 8, 2, UART_PARITY_NONE);
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_RISE, true, interrupt);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

#endif
