**FRC 7902 LED Light Strip Program**

This program allows the RIO to communicate with a Raspberry Pi Pico, which outputs a colour/effect to the LED strip connected to the Raspberry Pi Pico.


A command is sent in this order (do not have any spaces in the input):
1,2,3,4,5

1: Brightness (from 0 - 255)
2: Mode (from 1 - 5)
3: R (Red value)
4: G (Green value)
5: B (Blue value)


**Modes**:
1: Static Lights
2: Flashing Lights
3: Rainbow Lights
4: Fading Lights
5: Pattern Lights (I don't know what to properly call this one) 


NOTE:
- Current communication protocol used: **UART**
- Please only use the Arduino file of the program (light_uartC.ino). 
- To turn off all lights, only input 0.
- Rainbow Lights do not need RGB values. You can just input the brightness value and the mode (which would be 3).
- Some modes will not be affected by the brightness values. These modes are:
  
  1. Rainbow Lights
  2. Fading Lights
  3. Pattern Lights


**System Information**
Microcontroller: Raspberry Pi Pico (RP2040)
Language: C++ (Arduino version)
Receive Pin (UART) = DIO 1
LED DATA Pin = DIO 2
IRQ Pin = DIO 3
Baud rate = 300


**Thank you Davis and Arjun for all your help :)**
