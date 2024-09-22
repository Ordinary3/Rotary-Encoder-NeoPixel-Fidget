# Arduino Neopixel Rotary Fidget Toy
A fun fidget thing made with Arduino IDE with Neopixels  
designed using Kicad v.6 and uses Neopixel.h   
  
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

if you dont want to use the custom pcb just use a breadboard and some jumper wires  
Pin 6: Controls the NeoPixel ring. (Din)  
Pin 2: Encoder Pin A (for reading rotary encoder position).  
Pin 3: Encoder Pin B (for reading rotary encoder position).  
Pin 4: Rotary encoder button (used for mode switching).  
Pin 5: Vibration motor control.  
Pin 7: Passive buzzer control.  
then just plug the VCC and GND as usual

### Special thanks to pdp7  
without them this project wouldnt exists  
https://github.com/pdp7/rotary-encoder-breakout  
  
