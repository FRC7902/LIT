#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/uart.h"
#include "ws2812/generated/ws2812.pio.h"

#define UART_ID uart0
#define BAUD_RATE 300
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define WS2812_PIN 2
#define NUM_PIXELS 20
#define IS_RGBW false
#define IRQ_PIN 3

#define LED_DELAY_MS 2000;

void pico_led_init(void) {
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
}

char mode, brightness, r, g, b, stringy[5];
bool irq_flag = false, direction = true, isToBeTurnedOn = true;
int i, bytes_transmitted;

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

static inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            ((uint32_t) (w) << 24) |
            (uint32_t) (b);
}

void emitStaticColourAll(int r, int g, int b, int brightness) {
    put_pixel(urgb_u32(r, g, b));
}

void emitStaticColour(int r, int g, int b, int brightness, int ledIndex) {
    put_pixel(urgb_u32(r, g, b));
}

void flashingLights(int r, int g, int b, int brightness) {
  emitStaticColourAll(0, 0, 0, 0);
  while (irq_flag == false) {
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(500000);
    emitStaticColourAll(0, 0, 0, 0);
    sleep_ms(500000);
  }
}

void rainbowLights(int brightness) {
  i = 0;
  emitStaticColourAll(0, 0, 0, 0);
  while (irq_flag == false) {
    i = (direction == true) ? (i + 1) : (i - 1);
    emitStaticColourAll(i, 0, 255 - i, brightness);
    sleep_ms(30000);
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
    sleep_ms(1000);
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
    sleep_ms(100000);
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


int main()
{   
    gpio_set_irq_enabled_with_callback(IRQ_PIN, GPIO_IRQ_EDGE_RISE, true, interrupt);

    stdio_init_all();
    gpio_set_dir(IRQ_PIN, GPIO_OUT);   
    gpio_pull_down(IRQ_PIN);
    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  
    pico_led_init();
    //gpio_put(25, true);
    while (true)
    {
        if (uart_is_readable(UART_ID))
        {
            //gpio_put(25, true);
            printf("yay");
            brightness = mode = r = g = b = 0;
            // Serial.println(bytes_transmitted);
            irq_flag = false;
            brightness = (bytes_transmitted >= 0) ? (uart_getc(UART_ID)) : (0);
            //if (mode == 0) {
              //gpio_put(25, true);
            //}
            mode = (bytes_transmitted >= 1) ? (uart_getc(UART_ID)) : (0);
            r = (bytes_transmitted >= 2) ? (uart_getc(UART_ID)): (0);
            g = (bytes_transmitted >= 3) ? (uart_getc(UART_ID)) : (0);
            b = (bytes_transmitted >= 4) ? (uart_getc(UART_ID)) : (0);

            sleep_ms(500);

            sprintf(stringy, "%d", brightness);
            printf(stringy);

            sprintf(stringy, "%d", mode);
            printf(stringy);

            sprintf(stringy, "%d", r);
            printf(stringy);

            sprintf(stringy, "%d", g);
            printf(stringy);

            sprintf(stringy, "%d", b);
            printf(stringy);
            

            mode = 1;
            if (brightness == 0)
            {
                emitStaticColourAll(0, 0, 0, 0);
            }

            else if (mode == 1)
            {
                gpio_put(25, true);
                emitStaticColourAll(0, 0, 0, 0);
                emitStaticColourAll(255, 255, 255, 100);
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
        // Use some the various UART functions to send out data
        // In a default system, printf will also output via the default UART

        // Send out a string, with CR/LF conversions

        // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart
    
}
