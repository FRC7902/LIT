from machine import UART, Pin
from ws2812 import WS2812

test_uart2 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
ws = WS2812(machine.Pin(2),20)

def lightstrip(r, g, b):
    for i in range(0, 19):
        ws[i] = [r, g, b]
    ws.write()    
    


while True:
    if test_uart2.any(): #checks if there's data being sent
        input = test_uart2.read().decode()
        
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

