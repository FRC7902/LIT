#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_RX_PIN 1
#define WS2812_PIN 2
#define IRQ_PIN 3

int mode, brightness, step, stringy[5];
bool irq_flag = false, direction = true;
float i, r, g, b;
int NUM_PIXELS = 20;
uint32_t colour_set;



float rainbowLightConstant = 0.3;

int flashingLightDelay = 500,
    rainbowLightDelay = 3,
    patternLightDelay = 50,
    fadingLightDelay = 2;

//values are in milliseconds

#endif