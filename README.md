## Arduino Neopixel Rotary Fidget Toy
#Overview

This project utilizes the Adafruit NeoPixel library to control a NeoPixel ring with various modes of operation, including color cycling, brightness adjustment, and strobe effects. The primary interface is a rotary encoder, which allows users to navigate between different modes and adjust parameters.

PCB was designed using Kicad v.6
  
Might remake the whole code using Fastled.h  
  
This is my first ever arduino project that uses a custom pcb but **this project is still possible without it.**
  
## Required Parts:  
1x Arduino Nano or LGT8F328p  
1x EC11 Rotary Encoder OR a EC11 Rotary Encoder with breakout pcb  
1x Neopixel 16 Ring (Go ahead if you want to use other versions like the 24 and so on)  
1x Passive Buzzer   
1x Vibration Module  
some awg 26 wires
  
**Optional:**  
1x Switch (Power On/Off)  
1x Charging Module (Ex. TP4056)  
2x Button (for Reset and Mode Change) 

#Features
Rotary Encoder Control: Navigate through modes and adjust settings using a rotary encoder.
Color Modes: Various color display modes including:
Rainbow cycle
Rainbow wave
Rainbow pulse
Color selection via encoder
Brightness Adjustment: Adjust brightness using the encoder.
Strobe Effect: Randomly activate strobe lights.
RGB Slider: Smooth transition between RGB colors.

#Wiring Diagram (for breadboard)
Connect the components as follows:

NeoPixel data pin to pin 6
Rotary encoder pins A and B to pins 2 and 3 respectively
Encoder button to pin 4
Mode button to pin 7
Vibration motor to pin 5
Ensure to connect ground and power accordingly.

### Special thanks to pdp7  
without them this project wouldnt exists  
https://github.com/pdp7/rotary-encoder-breakout  
  
