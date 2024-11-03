**RELEASE NOTES: Version 2 🎉**
=====================================

**Summary**
-----------

Version 2 brings significant enhancements to the Arduino Neopixel Rotary Fidget Toy project, including:

* **Improved Structure**: Enhanced guide for easier navigation
* **Custom Modes**: Expanded functionality for a more engaging experience
* **Readability Updates**: Cleaner code for better understanding
* **Beginner-Friendly**: Simplified setup and operation for new users

**Important Notes**
-------------------

* **Compatibility**: If not using a custom PCB, consider sticking with Version 1 or adapting the Read Encoder from Version 1. some breakout  support Version 2.
* **Documentation**: Additional resources are pending and will be updated soon.
* **Licensing**: 
	+ **Commercial Use**: Prohibited without explicit permission.
	+ **Educational/Personal Use**: Permitted for learning and personal projects.

**Project Overview**
--------------------

### Arduino Neopixel Rotary Fidget Toy

* **Description**: Control a NeoPixel ring with various modes (color cycling, brightness adjustment, strobe effects) using a rotary encoder.
* **Design Tools**: KiCad v.6 (PCB design)
* **Future Plans**: Potential code rewrite using FastLED.h

**Required Components**
------------------------

1. **Microcontroller**: Arduino Nano or LGT8F328p
2. **Rotary Encoder**: EC11 (with or without breakout PCB)
3. **NeoPixel Ring**: 16 LEDs (you can use any version just edit #define NUM_PIXELS [n])
4. **Vibration Module**
5. **AWG 26 Wires**
6. **Button** (2) (For Mode switch and Reset)

**Optional Components**
-----------------------

1. **Slide Switch**
2. **Charging Module** (e.g., TP4056)
3. **Lipo Batt**
4. **Passive Buzzer** (Optional)

**Key Features**
----------------

* **Rotary Encoder Control**: Navigate modes and adjust settings
* **Color Modes**:
	+ Rainbow Cycle
	+ Rainbow Wave
	+ Rainbow Pulse
	+ Encoder-driven Color Selection
* **Brightness Adjustment**: Encoder-controlled
* **Strobe Effect**: Random strobe light activation
* **RGB Slider**: Smooth RGB color transitions
* **NEW Memory Game**: Identify LED patterns to win

**Breadboard Wiring Diagram**
-----------------------------

| Component | Arduino Pin |
| --- | --- |
| NeoPixel Data | 6 |
| Rotary Encoder A | 2 |
| Rotary Encoder B | 3 |
| Encoder Button | 4 |
| Mode Button | 7 |
| Vibration Motor | 5 |
| **Remember to connect Ground and Power accordingly.** |

**Acknowledgments**
------------------

* **Special Thanks to [pdp7](https://github.com/pdp7/rotary-encoder-breakout)**: This project is initially forked from their breakout board design.
