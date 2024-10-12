#include <stdio.h>
#include <stdlib.h>


#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/uart.h"
#include "math.h"
#include "hardware/clocks.h"
#include "ws2812/ws2812.pio.h"

#define UART_ID uart0
#define BAUD_RATE 300
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define WS2812_PIN 2
#define IRQ_PIN 3

struct RGB{
uint8_t r;
uint8_t g;
uint8_t b;
};

int mode, brightness, r, g, b, NUM_PIXELS = 20, stringy[5];
bool irq_flag = false, direction = true, isToBeTurnedOn = true;
int bytes_transmitted;
float i;
uint32_t colour_set;

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         (uint32_t)(b);
}

void emitStaticColourAll(double r, double g, double b, double brightness) {
      colour_set = urgb_u32((r), (g), (b));
      for (int x = 0; x < NUM_PIXELS; x++) {
        put_pixel(colour_set);
      }
}

void emitStaticColour(double r, double g, double b, double brightness, int ledIndex) {
    for (int g = ledIndex; g != 0 || g != 20; g--) {
      put_pixel(colour_set);
    }
    colour_set = urgb_u32((r), (g), (b));
    put_pixel(colour_set);
}

void flashingLights(int r, int g, int b, int brightness) {
  while (irq_flag == false) {
    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(500);
    emitStaticColourAll(0, 0, 0, 0);
    sleep_ms(500);
  }
}

void rainbowLights(double r, double g, double b, int brightness) {
  i = 0.2;
  r = 255.0;
  g = 255.0;
  b = 255.0;
  int step = 1, ranOnce = 0;
  irq_flag = false;
  direction = true;
  while (irq_flag == false) {
    //i = (direction == true) ? (i + 0.02) : (i - 0.02);
    i += 0.01;

    if (step == 1) {
      if (direction == true) {
        r -= i;
      }
      else if (direction == false) {
        r += i;
      }
    }
    else if (step == 2) {
      if (direction == true) {
        r += i;
        g -= i;
      }
      else if (direction == false) {
        r -= i;
        g += i;
      }
    }

    else if (step == 3) {
      if (direction == true) {
        g += i;
        b -= i;
      }
      else if (direction == false) {
        g -= i;
        b += i;
      }

    }

    emitStaticColourAll(r, g, b, brightness);
    sleep_ms(25);

            sprintf(stringy, "%d", step);
            printf(stringy);
            printf("\nR:\n\n");
    if (i >= 3.4) {
      if (step == 3 && direction == true) {
        direction = false;
        step++;
      }
      else if (step == 1 && direction == false) {
        direction = true;
        step--;
      }
      if (direction == false) {
        i = 0.2;
        step--;  
      }
      else if (direction == true) {
        i = 0.2;
        step++;
      }       
    }
  }
}

void fadingLights(int r, int g, int b, int brightness) {
  i = 0;
  direction = true;
  while (irq_flag == false) {
            sprintf(stringy, "%d", r);
            printf(stringy);
            printf("\nR:\n\n");        
    if (direction == true) {
      i++;
      r = (r == 0) ? (r) : (r - 1);
      g = (g == 0) ? (g) : (g - 1);
      b = (b == 0) ? (b) : (b - 1);
    }
    else if (direction == false) {
      i--;
      r = (r == 0) ? (r) : (r + 1);
      g = (g == 0) ? (g) : (g + 1);
      b = (b == 0) ? (b) : (b + 1);
    }

    emitStaticColourAll(r, g, b, i);
    sleep_ms(5);
    if (i == 250) {
      direction = false;
    }
    else if (i == 1) {
      direction = true;
    }
  }
}

void patternLights(int r, int g, int b, int brightness) {
  i = 0;
  while (irq_flag == false) {
    emitStaticColour(r, g, b, brightness, i);
    sleep_ms(100);
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
    printf("\nYOU INTERRUPTED BITCH");
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

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (true)
    {
        if (uart_is_readable(UART_ID))
        {
            //emitStaticColourAll(0, 0, 0, 0);
            printf("yey\n");
            brightness = mode = r = g = b = 0;
            irq_flag = false;

            brightness = uart_getc(UART_ID);
            mode = uart_getc(UART_ID);
          
            r = uart_getc(UART_ID);
            g = uart_getc(UART_ID);
            b = uart_getc(UART_ID);

            //mode = (bytes_transmitted >= 1) ? ((uart_getc(UART_ID)) / 10000) : (0);
           // r = (bytes_transmitted >= 2) ? ((uart_getc(UART_ID)) / 10000): (0);
            //g = (bytes_transmitted >= 3) ? ((uart_getc(UART_ID)) / 10000) : (0);
            //b = (bytes_transmitted >= 4) ? ((uart_getc(UART_ID)) / 10000) : (0);

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
           

            if (brightness == 0)
            {
                irq_flag = false;
                emitStaticColourAll(0, 0, 0, 0);
            }

            else if (mode == 1)
            { 
                irq_flag = false;
                emitStaticColourAll(r, g, b, 100);
            }

            else if (mode == 2)
            {
                irq_flag = false;
                flashingLights(r, g, b, brightness);
            }

            else if (mode == 3)
            {
                irq_flag = false;
                rainbowLights(r, g, b, brightness);
            }

            else if (mode == 4)
            {
                irq_flag = false;
                fadingLights(r, g, b, brightness);
            }

            else if (mode == 5)
            {
                irq_flag = false;
                patternLights(r, g, b, brightness);
            }
        }
    }
    
}
