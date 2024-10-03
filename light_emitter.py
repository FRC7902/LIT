from machine import UART, Pin
from ws2812 import WS2812

test_uart2 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1)) #declaration of uart connection (uart port, baudrate, tx, rx)
ws = WS2812(machine.Pin(2),20) #declaration of led strip (pin, # of leds)

def lightstrip(r, g, b): #function to change colours on lightstrip
    for i in range(0, 19): 
        ws[i] = [r, g, b] #updates all the leds on the rgb strip to one colour, may be changed to have multiple colours
    ws.write() #turns the specified leds on with the specified colour
    

#r8js in a loop so that light values will change if theres a new input value
while True:
    if test_uart2.any(): #checks if there's data being sent
        
        input = test_uart2.read().decode() #stores inputted value

        #test values to see if the pico will update the colour based on the input it receives
        if input == "1":
            lightstrip(255, 0, 0)
        elif input == "2":
            lightstrip(0, 0, 255)
        elif input == "3":
            lightstrip(255, 255, 0)
        elif input == "4":
            lightstrip(0, 255, 0)
        elif input == "5":
            lightstrip(255, 165, 0)
        else:
            pass

