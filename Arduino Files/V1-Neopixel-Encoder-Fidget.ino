//Originaly made by ordinarydev/ordinary2284 please credit if used!

#include <Adafruit_NeoPixel.h>
#include <Encoder.h>

#define PIN_NEO_PIXEL 6    // Pin for NeoPixel
#define NUM_PIXELS 16      // Number of LEDs in the NeoPixel ring
#define ENCODER_PIN_A 2    // Encoder pin A
#define ENCODER_PIN_B 3    // Encoder pin B
#define ENCODER_BTN 4      // Rotary encoder button pin
#define VIBRATION_MOTOR_PIN 5  // Pin for vibration motor
#define BUZZER_PIN 7  // Pin for the passive buzzer

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Encoder myEnc(ENCODER_PIN_A, ENCODER_PIN_B);

int mode = 0;                // Current mode
long oldPosition = -999;     // Track previous encoder position
int brightness = 125;        // Global brightness (0-255)
bool menuActive = false;     // Flag to indicate if menu is active
uint16_t rainbowStep = 0;    // Step variable for rainbow animation
int breathStep = 0;          // Step for breathing effect

void setup() {
  strip.begin();             // Initialize NeoPixel
  strip.show();              // Ensure the strip is cleared
  pinMode(ENCODER_BTN, INPUT_PULLUP);  // Button for mode switching
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);  // Initialize vibration motor pin
  randomSeed(analogRead(0));  // Seed random function
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Handle menu and mode switching with button press
  if (digitalRead(ENCODER_BTN) == LOW) {
    delay(50); // Short delay for debounce
    while (digitalRead(ENCODER_BTN) == LOW) {
      delay(10); // Wait for button release
    }
    
    if (!menuActive) {
      menuActive = true;
      showMenu(); // Show menu when button is held
    } else {
      mode = (mode + 1) % 8;   // Cycle between modes 0-7
      menuActive = false;
      strip.clear();
      delay(300);             // Debounce button
    }
  }

  long newPosition = myEnc.read() / 4;  // Read encoder position (adjust scaling if needed)

  if (!menuActive) {
    // Update brightness if in mode 1
    if (mode == 1) {
      brightness = map(newPosition, 0, 255, 0, 255);
    }

    // Only update LEDs if the encoder position changes or in certain modes
    if (newPosition != oldPosition || mode == 2 || mode == 3 || mode == 4 || mode == 5 || mode == 6 || mode == 7) {
      switch (mode) {
        case 0:
          modeFollowRotaryMirrored(newPosition);
          break;
        case 1:
          modeBrightnessControl();
          break;
        case 2:
          modeRainbowAnimation();
          break;
        case 3:
          modeRandomStrobe();
          break;
        case 4:
          modeBreathingAnimation();  // Breathing animation with vibration
          break;
        case 5:
          modeChaseAnimation();  // Chase animation
          break;
        case 6:
          modeFlashbang();  // Flashbang effect
          break;
        case 7:
          modeRainbowCycleControl(newPosition);  // New mode for controlling rainbow cycle
          break;
        default:
          break;
      }
      oldPosition = newPosition;
    }
  }
}

// Show the menu to select modes
void showMenu() {
  // Define colors for each mode
  const uint32_t modeColors[] = {
    strip.Color(255, 255, 255),  // White for Mode 0
    strip.Color(0, 255, 0),      // Green for Mode 1
    strip.Color(255, 0, 0),      // Red for Mode 2
    strip.Color(0, 0, 255),      // Blue for Mode 3
    strip.Color(255, 255, 0),    // Yellow for Mode 4 (Breathing)
    strip.Color(255, 0, 255),    // Magenta for Mode 5 (Chase)
    strip.Color(255, 255, 255),  // White for Mode 6 (Flashbang)
    strip.Color(0, 255, 255)     // Cyan for Mode 7 (Rainbow Cycle Control)
  };

  // Initialize encoder position
  long lastEncoderPosition = myEnc.read() / 4;
  int highlightedMode = mode;  // Start by highlighting the current mode

  // Display the menu
  while (digitalRead(ENCODER_BTN) == HIGH) {
    // Read the current encoder position
    long currentEncoderPosition = myEnc.read() / 4;

    // Check if the encoder position has changed
    if (currentEncoderPosition != lastEncoderPosition) {
      // Calculate mode change
      int modeChange = currentEncoderPosition - lastEncoderPosition;
      highlightedMode = (highlightedMode + modeChange) % 8;
      if (highlightedMode < 0) highlightedMode += 8;  // Ensure mode wraps around

      // Clear the LED strip
      strip.clear();
      
      // Highlight the LED for the currently selected mode
      for (int i = 0; i < 8; i++) {
        if (i == highlightedMode) {
          strip.setPixelColor(i, modeColors[i]);  // Highlight the selected mode
        } else {
          strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off other LEDs
        }
      }
      strip.show();

      // Update last encoder position
      lastEncoderPosition = currentEncoderPosition;
    }

    delay(25);  // Small delay for smooth scrolling
  }

  // Set the selected mode after button release
  mode = highlightedMode;
  delay(300);  // Debounce delay
}

// Mode 0: Follow Rotary Encoder with Mirrored Effect
void modeFollowRotaryMirrored(long pos) {
  int ledPos = (pos % NUM_PIXELS + NUM_PIXELS) % NUM_PIXELS;  // Ensure position wraps around within 16 LEDs
  int mirroredPos = NUM_PIXELS - 1 - ledPos;  // Mirrored position across the NeoPixel ring

  int ledsOnCount = 0;  // Variable to count how many LEDs are turned on
  for (int i = 0; i < NUM_PIXELS; i++) {
    if (i <= mirroredPos) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));  // Set LED to white with global brightness
      ledsOnCount++;  // Increment the count of LEDs that are turned on
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off remaining LEDs
    }
  }

  strip.show();  // Update the NeoPixel ring

  int buzzerFrequency = map(ledsOnCount, 0, NUM_PIXELS, 500, 2000);  // Frequency range from 500Hz to 2000Hz
  
  if (pos != oldPosition) {
    digitalWrite(VIBRATION_MOTOR_PIN, HIGH);  // Turn on motor
    tone(BUZZER_PIN, buzzerFrequency);  // Play buzzer sound at the calculated frequency
    delay(25);  // Adjust vibration and buzzer duration as needed
    digitalWrite(VIBRATION_MOTOR_PIN, LOW);   // Turn off motor
    noTone(BUZZER_PIN);  // Stop buzzer sound
  }
}

// Mode 1: Brightness Control
void modeBrightnessControl() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));  // Set LEDs to white with global brightness
  }

  strip.show();  // Update the NeoPixel ring
}

// Mode 2: Rainbow Animation
void modeRainbowAnimation() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    int pixelHue = (rainbowStep + (i * 65536L / NUM_PIXELS)) & 0xFFFF;  // Calculate hue
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));     // Set LED to rainbow color
  }
  strip.show();
  delay(15);  // Control the speed of the rainbow animation
  rainbowStep += 256;  // Advance the rainbow step for the next iteration
}

// Mode 3: Random Strobe
void modeRandomStrobe() {
  uint32_t colors[] = {
    strip.Color(255, 0, 0),    // Red
    strip.Color(255, 255, 255),// White
    strip.Color(0, 0, 255)     // Blue
  };

  int strobeCount = 0;  // Counter for strobe flashes

  while (true) {
    if (digitalRead(ENCODER_BTN) == LOW) {
      delay(50);  // Debounce delay
      while (digitalRead(ENCODER_BTN) == LOW) {
        delay(10);  // Wait for the button to be released
      }
      strip.clear();
      strip.show();
      return;  // Exit the strobe mode
    }

    int selectedLEDs[9];
    for (int i = 0; i < 9; i++) {
      bool unique = true;
      int newColor;
      do {
        newColor = colors[random(0, 3)];
        for (int j = 0; j < i; j++) {
          if (selectedLEDs[j] == newColor) {
            unique = false;
            break;
          }
        }
      } while (!unique);
      selectedLEDs[i] = newColor;
    }

    for (int i = 0; i < NUM_PIXELS; i++) {
      strip.setPixelColor(i, selectedLEDs[i % 9]);  // Cycle through colors
    }
    strip.show();
    delay(random(50, 150));  // Random delay between flashes
  }
}

// Mode 4: Breathing Animation with Vibration
void modeBreathingAnimation() {
   static unsigned long lastUpdate = 0;
  unsigned long currentMillis = millis();
int brightness = 0;           // Current brightness level
int fadeAmount = 5;           // Amount to change brightness each cycle
int breathingDelay = 10;   
  // Update brightness at regular intervals
  if (currentMillis - lastUpdate >= breathingDelay) {
    lastUpdate = currentMillis;

    // Use a sinusoidal function to create a smoother breathing effect
    float time = (currentMillis % 2000) / 2000.0; // Cycle every 2000ms (2 seconds)
    brightness = (sin(time * 2 * PI) + 1) * 127.5; // Convert sine wave to 0-255 range

    // Update LED colors
    for (int i = 0; i < NUM_PIXELS; i++) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness)); // Set LEDs to white with current brightness
    }

    strip.show();
  }
}

// Mode 5: Chase Animation
void modeChaseAnimation() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off all LEDs
  }

  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));  // Turn on the current LED
    strip.show();
    delay(50);  // Adjust speed of the chase effect
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off the current LED
  }
}

// Mode 6: Flashbang Effect
void modeFlashbang() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    int pixelHue = (rainbowStep + (i * 65536L / NUM_PIXELS)) & 0xFFFF;  // Calculate hue
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));     // Set LED to rainbow color
  }
  strip.show();
  delay(1);  // Control the speed of the rainbow animation
  rainbowStep += 1245;  // Advance the rainbow step for the next iteration
}

// Mode 7: Rainbow Cycle Control
void modeRainbowCycleControl(long pos) {
  // Map encoder position to speed of the rainbow animation
  int speed = map(pos, 0, 255, 1, 100);  // Speed ranges from 1 to 100ms delay
  static unsigned long lastUpdate = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= speed) {
    lastUpdate = currentMillis;

    // Update rainbow animation
    for (int i = 0; i < NUM_PIXELS; i++) {
      int pixelHue = (rainbowStep + (i * 65536L / NUM_PIXELS)) & 0xFFFF;  // Calculate hue
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));     // Set LED to rainbow color
    }
    strip.show();
    rainbowStep += 256;  // Advance the rainbow step for the next iteration
  }
}
