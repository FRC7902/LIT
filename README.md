# **FRC 7902 LED Light Strip Program** #
<br />
This program allows the RIO to communicate with a Raspberry Pi Pico, which outputs a colour/effect to the LED strip connected to the Raspberry Pi Pico.<br />
<br />
A command is sent in this order (do not have any spaces in the input):<br />

**1,2,3,4,5**
<br />
<br />

**1: Brightness** (from 0 - 255)
<br />

**2: Mode** (from 1 - 5)
<br />

**3: R** (Red value)
<br />

**4: G** (Green value)
<br />

**5: B** (Blue value)
<br />
<br />

# **Modes**: #
1: **Static Lights**
<br />

2: **Flashing Lights**
<br />

3: **Rainbow Lights**
<br />

4: **Fading Lights**
<br />

5: **Pattern Lights** (I don't know what to properly call this one)<br />

<br />


**NOTE:**
- Current communication protocol used: **UART**
- You can either use the C or the Arduino file. Both files are functional. 
- To turn off all lights, input 0,1,0,0,0.
- Rainbow Lights are not affected by RGB values, however, you still need to input the RGB values (just put 0 for each). This ensures the buffer gets the right amount of data.
- Brightness is a W.I.P as of now.
- 
<br />

# **System Information** #
<br />

Microcontroller: Raspberry Pi Pico (RP2040)
<br />

Language: C / C++ (Arduino version)
<br />

Receive Pin (UART) = DIO 1
<br />

LED TX Pin = DIO 2
<br />

IRQ Pin = DIO 3
<br />

Baud rate = 300
<br />


**Thank you Davis and Arjun for all your help :)**
