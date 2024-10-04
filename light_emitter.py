from machine import UART, Pin
from ws2812 import WS2812
import utime

rio = UART(0, baudrate = 4800, tx = Pin(0), rx = Pin(1))
ws = WS2812(machine.Pin(2),20) #60 leds, to change later

#Connect TX to Pin  (1)
#Connect RX to Pin 1 (2)
#Connect DATA Pin of WS2812 to Pin 2 (4)

def emitStaticColour(r, g, b):
    for i in range(0, 19):
        ws[i] = [r, g, b]
    ws.write()

def flashingLights(r, g, b):
    emitStaticColour(r, g, b)
    utime.sleep(0.5)
    emitStaticColour(0, 0, 0)
    utime.sleep(0.5)

def emitAlternatingColour(r, g, b, r2, g2, b2):
    emitStaticColour(r, g, b)
    utime.sleep(1)
    emitStaticColour(r2, g2, b2)
    utime.sleep(1)

emitStaticColour(0, 0, 0)

while True:
    if rio.any(): #checks if there's data being sent
        input = rio.read().decode().split(",")
        if input[0] == 'end':
            emitStaticColour(0, 0, 0)
            break
        
        r, g, b, pattern = int(input[0]), int(input[1]), int(input[2]), int(input[3])
            
        if pattern == 0: #static colour mode
            emitStaticColour(r, g, b)
            
        elif pattern == 1: #flashing light mode
            while True:
                interrupt = rio.read()
                if interrupt:
                    interrupt = interrupt.decode("utf-8")
                if interrupt == "stop":
                    break
                flashingLights(r, g, b)
        
        elif pattern == 2: #alternating colour mode
            while True:
                interrupt = rio.read()
                if interrupt:
                    interrupt = interrupt.decode("utf-8")
                if interrupt == "stop":
                    break
                r2, g2, b2 = int(input[4]), int(input[5]), int(input[6])
                emitAlternatingColour(r, g, b, r2, g2, b2)
                
          

