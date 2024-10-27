## **VERSION 2 HAS BEEN RELEASED 🎉**  
Version 2 introduces significant improvements, including a well-structured guide, custom modes, enhanced readability, and a more beginner-friendly experience. If you are not using a custom PCB, we recommend sticking with Version 1 or modifying the Read Encoder from Version 1 to Version 2. Please note that additional documentation is still pending.

Commercial use is prohibited without explicit permission. However, you are welcome to use this project for educational purposes or personal development.

### Arduino Neopixel Rotary Fidget Toy
**Overview**

This project leverages the Adafruit NeoPixel library to control a NeoPixel ring, offering various operational modes such as color cycling, brightness adjustment, and strobe effects. The main interface utilizes a rotary encoder, enabling users to seamlessly navigate between different modes and adjust settings.

The PCB was designed using KiCad v.6, and there are plans to potentially rewrite the entire code using FastLED.h. While this is my first Arduino project featuring a custom PCB, it is important to note that the project can still be implemented without one.
  
## Required Parts:  
1x Arduino Nano or LGT8F328p  
1x EC11 Rotary Encoder OR a EC11 Rotary Encoder with breakout pcb  
1x Neopixel 16 Ring (Go ahead if you want to use other versions like the 24 and so on)  
1x Passive Buzzer   (Optional)
1x Vibration Module  
some awg 26 wires
  
**Optional:**  
1x Switch (Power On/Off)  
1x Charging Module (Ex. TP4056)  
2x Button (for Reset and Mode Change) 

## Features:   
Rotary Encoder Control: Navigate through modes and adjust settings using a rotary encoder.
Color Modes: Various color display modes including:
Rainbow cycle
Rainbow wave
Rainbow pulse
Color selection via encoder
Brightness Adjustment: Adjust brightness using the encoder.
Strobe Effect: Randomly activate strobe lights.
RGB Slider: Smooth transition between RGB colors.

## Wiring Diagram (for breadboard):  
Connect the components as follows:  

NeoPixel data pin to pin 6  
Rotary encoder pins A and B to pins 2 and 3 respectively  
Encoder button to pin 4  
Mode button to pin 7  
Vibration motor to pin 5  
Ensure to connect ground and power accordingly.  

### Special thanks to pdp7  
Special thanks to [pdp7](https://github.com/pdp7/rotary-encoder-breakout) for their invaluable contributions; this project would not exist without their support. The original pcb was a fork of their breakout board
  
