---

## **Version 2 Released 🎉**  

Version 2 introduces significant improvements, including a well-structured guide, custom modes, enhanced readability, and a more beginner-friendly experience. If you are not using a custom PCB, we recommend sticking with Version 1 or modifying the Read Encoder from Version 1 to Version 2. Please note that additional documentation is still pending.

**Commercial use is prohibited without explicit permission. However, you are welcome to use this project for educational purposes or personal development.**

### Arduino NeoPixel Rotary Fidget Toy

**Overview**

This project leverages the Adafruit NeoPixel library to control a NeoPixel ring, offering various operational modes such as color cycling, brightness adjustment, and strobe effects. The main interface utilizes a rotary encoder, enabling users to seamlessly navigate between different modes and adjust settings.

The PCB was designed using KiCad v.6, and there are plans to potentially rewrite the entire code using FastLED.h. While this is my first Arduino project featuring a custom PCB, it is important to note that the project can still be implemented without one.

## Required Parts:  
- 1x Arduino Nano or LGT8F328P  
- 1x EC11 Rotary Encoder or EC11 Rotary Encoder with breakout PCB  
- 1x 16-LED NeoPixel Ring (other sizes like 24-LED versions are compatible)  
- 1x Passive Buzzer (optional)  
- 1x Vibration Module  
- Some AWG 26 wires  

**Optional:**  
- 1x Switch (Power On/Off)  
- 1x Charging Module (e.g., TP4056)  
- 2x Buttons (for Reset and Mode Change)  

## Features:   
- **Rotary Encoder Control:** Navigate through modes and adjust settings using the rotary encoder.  
- **Color Modes:** Includes various display modes such as:
  - Rainbow Cycle
  - Rainbow Wave
  - Rainbow Pulse
  - Color Selection via Encoder
- **Brightness Adjustment:** Adjust brightness using the encoder.
- **Strobe Effect:** Randomly activate strobe lights.
- **RGB Slider:** Achieve smooth transitions between RGB colors.

## Wiring Diagram (for Breadboard):  
Connect the components as follows:  
- NeoPixel data pin to pin 6  
- Rotary encoder pins A and B to pins 2 and 3, respectively  
- Encoder button to pin 4  
- Mode button to pin 7  
- Vibration motor to pin 5  

Ensure proper connections to ground and power.

### Special Thanks  
Special thanks to [pdp7](https://github.com/pdp7/rotary-encoder-breakout) for their invaluable contributions; this project would not exist without their support.

--- 
