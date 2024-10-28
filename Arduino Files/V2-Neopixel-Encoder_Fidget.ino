//Below is version 2 of my system almost completely rewritten to work with the custom pcb
//if there is any issues with Read encoder or if you are using the ec11 break-
//-out recommend using the version 1 Read Encoder!

#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 6        // Pin for NeoPixel
#define NUM_PIXELS 16          // Number of LEDs in the NeoPixel ring
#define ENCODER_A 2            // Encoder pin A
#define ENCODER_B 3            // Encoder pin B
#define ENCODER_BTN 4          // Rotary encoder button pin
#define MODE_BTN 7             // Mode selection button pin
#define VIBRATION_MOTOR_PIN 5  // Pin for vibration motor

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

int encoderPos = 0;            // Current position of the encoder
int lastStateA = HIGH;         // Last state of encoder pin A
int lastStateB = HIGH;         // Last state of encoder pin B
int mode = 0;                  // Current mode
int brightness = 255;          // Current brightness (0-255)
int rainbowMode = 0;           // Current rainbow animation mode
float hue = 0;                 // Current hue for color transition

// Menu variables
int menuMode = 0; // Variable to track the current menu mode

// New RGB slider variables
uint8_t red = 255;             // Starting red value
uint8_t green = 0;             // Starting green value
uint8_t blue = 0;              // Starting blue value
unsigned long lastUpdateTime = 0; 
int animationStep = 0;             
int RgbSliderMode = 0;
const int animationSpeed = 30; // Animation speed (ms)

// Global variables for strobe
unsigned long lastRandomFlash = 0;
unsigned long randomFlashTime = 0;
unsigned long randomFlashInterval = 50;  // Interval between flashes
unsigned long randomFlashDuration = 2500; // Duration of strobe in ms
bool randomStrobeActive = true;           // Active state for strobe


void setup() {
  pixels.begin();  // Initialize the NeoPixel ring
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);
  
  startupAnimation(); // Call the startup animation
}

void loop() {
  readEncoder();
  checkModeButton();
  checkEncoderButton();

  // Handle menu navigation only if in menu mode
  if (mode == -1) {
    handleMenu();
  } else {
    // Execute current mode's functionality
    switch (mode) {
      case 0: followEncoder(); break;          
      case 1: handleRainbowCycle(); break;            
      case 2: setColorWithEncoder(); break;     
      case 3: playRandomStrobeAnimation(); break; 
      case 4: adjustBrightness(); break; // Brightness control mode
      case 5: handleRgbSlider(); break; // Smooth RGB color scrolling
      case 6: theaterChase(); break; // Add this line for the new game mode
      case 7: rememberingGame(); break; // **New Mode: Remembering Game**
    }
  }
}

// Function for startup animation
void startupAnimation() {
  uint32_t colors[] = {
    pixels.Color(255, 0, 0),   // Red
    pixels.Color(0, 255, 0),   // Green
    pixels.Color(0, 0, 255),   // Blue
  };
  
  // Color cycling and fading
  for (int j = 0; j < 3; j++) {
    // Cycle through colors
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, colors[j]);
      pixels.show();
      delay(10);
    }
    
    // Fade out
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(
          (colors[j] >> 16 & 0xFF) * brightness / 255,
          (colors[j] >> 8 & 0xFF) * brightness / 255,
          (colors[j] & 0xFF) * brightness / 255
        ));
      }
      pixels.show();
      delay(10);
    }
  }

  // Transition from 16 LEDs to 1 LED
  for (int numPixels = NUM_PIXELS; numPixels > 0; numPixels--) {
    // Set all pixels to white up to the current numPixels
    for (int i = 0; i < numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Set to white
    }
    // Set remaining pixels to off
    for (int i = numPixels; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
    }
    pixels.show();
    delay(25); // Adjust delay for speed of descent
  }

  // Keep the last pixel on
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();
}

void handleMenu() {
  pixels.clear();

  // Set the LED color based on the selected menu mode
  switch (menuMode) {
    case 0:
      pixels.setPixelColor(menuMode, pixels.Color(255, 255, 255)); // Red for mode 0
      break;
    case 1:
      showRainbow(5); // Rainbow effect for mode 1
      return; // Skip default color setting
    case 2:
      showRainbow(1); // Rainbow effect for mode 2
      return; // Skip default color setting
    case 3:
      flashRedBlue(); // Rainbow effect for mode 3
      return; // Skip default color setting
    case 4:
      slowrise(); // Rainbow effect for mode 4
      return; // Skip default color setting
    case 5:
      showRainbow(20); // Rainbow effect for mode 5
      return; // Skip default color setting
    case 6:
      pixels.setPixelColor(menuMode, pixels.Color(255, 255, 255)); // Magenta for mode 6
      break;
    case 7:
      pixels.setPixelColor(menuMode, pixels.Color(0, 255, 0)); // Magenta for mode 6
      break;
  }

  pixels.show();
}

void readEncoder() {
  int currentStateA = digitalRead(ENCODER_A);
  int currentStateB = digitalRead(ENCODER_B);

  if (currentStateA == LOW && lastStateA == HIGH) {
    if (mode == 5) { // If in RGB slider mode
      encoderPos += (currentStateB == LOW) ? -1 : 1;
      encoderPos = (encoderPos + 256) % 256 ; // Loop from 0 to 255
    } else {
      encoderPos += (currentStateB == LOW) ? -1 : 1;
      encoderPos = (encoderPos + NUM_PIXELS) % NUM_PIXELS;
    }
    if (mode == -1) {
    menuMode += (currentStateB == LOW) ? -1 : 1; // Navigate through the menu
    menuMode = (menuMode + 8) % 8; // Wrap around the number of menu options add more modes here
    }

    vibrate();
  }

  lastStateA = currentStateA;
}

void checkModeButton() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(MODE_BTN);

  // When the mode button is pressed
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (mode == -1) { // If in menu mode
      // Confirm selection and set mode based on menu selection
      mode = menuMode;
      vibrate();
    } else {
      // Switch to menu mode
      mode = -1; // Use -1 to indicate menu mode
      vibrate();
    }
    delay(50); // Short debounce delay
  }

  lastButtonState = currentButtonState;
}

void checkEncoderButton() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(ENCODER_BTN);

  if (lastButtonState == HIGH && currentButtonState == LOW) { // Button pressed
  
    vibrate(); // Activate vibration motor
    if (mode == 1) { // If in RGB slider mode
      rainbowMode = (rainbowMode + 1) % 3; // Cycle through 3 rainbow animations
    } else if (mode == 5) { // If in RGB slider mode
      RgbSliderMode = (RgbSliderMode + 1) % 3; // Cycle through 3 rainbow animations
    }
    delay(50); // Short debounce delay
  }

  lastButtonState = currentButtonState;
}

// Function to show a rainbow effect
void showRainbow(int t) {
  for (int i = 0; i < 1; i++) {
    int hue = (i * 256 / NUM_PIXELS + millis() / t) % 256; // Calculate hue
    uint32_t color = Wheel(hue);
    pixels.setPixelColor(menuMode, color);
  }
  pixels.show();
}

// Function to show a rainbow effect
void flashRedBlue() {
  static unsigned long lastFlashTime = 0;
  static int colorIndex = 0; // Track the current color (0: Red, 1: Blue, 2: White)
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastFlashTime >= random(300, 1000)) { // Random delay
    lastFlashTime = currentMillis; // Update the last flash time
    
    pixels.clear(); // Clear previous colors

    switch (colorIndex) {
      case 0:
        pixels.setPixelColor(menuMode, pixels.Color(255, 0, 0)); // Red
        break;
      case 1:
        pixels.setPixelColor(menuMode, pixels.Color(0, 0, 255)); // Blue
        break;
      case 2:
        pixels.setPixelColor(menuMode, pixels.Color(255, 255, 255)); // White
        break;
    }
    
    pixels.show();
    colorIndex = (colorIndex + 1) % 3; // Cycle through 0, 1, 2
  }
}

void slowrise() {
  static int brightnessLevel = 0; // Current brightness level
  static int fadeDirection = 1;    // 1 for increasing, -1 for decreasing
  static unsigned long lastUpdateTime = 0; // To control timing

  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdateTime >= 30) { // Update every 30 ms
    lastUpdateTime = currentMillis;

    // Update brightness level
    brightnessLevel += fadeDirection * 5; // Change brightness by 5

    if (brightnessLevel >= 255) {
      brightnessLevel = 255;
      fadeDirection = -1; // Reverse direction
    } else if (brightnessLevel <= 0) {
      brightnessLevel = 0;
      fadeDirection = 1; // Reverse direction
    }

    pixels.clear();
    pixels.setPixelColor(menuMode, pixels.Color(brightnessLevel, brightnessLevel, brightnessLevel)); // Gray color
    pixels.show();
  }
}

void followEncoder() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, (i <= encoderPos) ? pixels.Color(brightness, brightness, brightness) : pixels.Color(0, 0, 0)); // White or off
  }
  pixels.show();
}

void vibrate() {
  digitalWrite(VIBRATION_MOTOR_PIN, HIGH); // Turn on the vibration motor
  delay(25); // Duration of the vibration
  digitalWrite(VIBRATION_MOTOR_PIN, LOW); // Turn off the vibration motor
}

void handleRainbowCycle() {
  switch (rainbowMode) {
    case 0: rainbowCycle(); break; // Original rainbow cycle
    case 1: rainbowWave(); break;  // New wave animation
    case 2: rainbowPulse(); break; // New pulse animation
  }
}

void rainbowCycle() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    int hue = (i * 256 / NUM_PIXELS + millis() / 10) % 256;
    uint32_t color = Wheel(hue);
    uint8_t r = (color >> 16 & 0xFF) * brightness / 255;
    uint8_t g = (color >> 8 & 0xFF) * brightness / 255;
    uint8_t b = (color & 0xFF) * brightness / 255;
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
  delay(10);
}

void rainbowWave() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    int wave = sin((i + millis() / 100.0) * 2 * PI / NUM_PIXELS) * 127 + 128; // Sine wave for smooth transition
    uint32_t color = Wheel(wave);
    uint8_t r = (color >> 16 & 0xFF) * brightness / 255;
    uint8_t g = (color >> 8 & 0xFF) * brightness / 255;
    uint8_t b = (color & 0xFF) * brightness / 255;
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
  delay(10);
}

void rainbowPulse() {
  static float pulseBrightness = 0.0;
  static float pulseDirection = 1.0;

  pulseBrightness += pulseDirection * 10.0; // Increased from 5.0 to 10.0 for faster pulsing
  if (pulseBrightness >= 255 || pulseBrightness <= 0) {
    pulseDirection *= -1; // Reverse direction
  }

  for (int i = 0; i < NUM_PIXELS; i++) {
    int hue = (i * 256 / NUM_PIXELS + millis() / 10) % 256;
    uint32_t color = Wheel(hue);
    uint8_t r = (color >> 16 & 0xFF) * pulseBrightness / 255;
    uint8_t g = (color >> 8 & 0xFF) * pulseBrightness / 255;
    uint8_t b = (color & 0xFF) * pulseBrightness / 255;
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
  delay(5); // Reduced delay for faster updates
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos; // Reverse the wheel to get the right colors
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void setColor(uint32_t color) {
  uint8_t r = (color >> 16 & 0xFF) * brightness / 255;
  uint8_t g = (color >> 8 & 0xFF) * brightness / 255;
  uint8_t b = (color & 0xFF) * brightness / 255;

  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();  
}

void setColorWithEncoder() {
  uint32_t colors[] = {
    pixels.Color(255, 0, 0),     // Red
    pixels.Color(0, 255, 0),     // Green
    pixels.Color(0, 0, 255),     // Blue
    pixels.Color(255, 255, 0),   // Yellow
    pixels.Color(255, 0, 255),   // Magenta
    pixels.Color(0, 255, 255),   // Cyan
    pixels.Color(255, 255, 255), // White
    pixels.Color(128, 0, 0),     // Maroon
    pixels.Color(0, 128, 0),     // Dark Green
    pixels.Color(0, 0, 128),     // Navy
    pixels.Color(128, 128, 0),   // Olive
    pixels.Color(128, 0, 128),   // Purple
    pixels.Color(0, 128, 128),   // Teal
    };

  int numColors = sizeof(colors) / sizeof(colors[0]);
  int colorIndex = encoderPos % numColors; // Select color based on encoder position
  setColor(colors[colorIndex]);
}

void playRandomStrobeAnimation() {
  unsigned long currentTime = millis();

  if (currentTime - randomFlashTime >= randomFlashInterval) {
    pixels.clear(); // Clear previous colors

    for (int i = 0; i < NUM_PIXELS; i += 3) {
      if (random(0, 10) > 5) { // Random chance to light this group
        int color = random(0, 3); // Random color index
        switch (color) {
          case 0: setColorGroup(i, pixels.Color(255, 0, 0)); // Red
            break;
          case 1: setColorGroup(i, pixels.Color(0, 0, 255)); // Blue
            break;
          case 2: setColorGroup(i, pixels.Color(255, 255, 255)); // White
            break;
        }
      }
    }

    pixels.show(); // Show the colors
    randomFlashTime = currentTime; // Update random flash timer
  }
}

void setColorGroup(int index, uint32_t color) {
  uint8_t r = (color >> 16 & 0xFF) * brightness / 255;
  uint8_t g = (color >> 8 & 0xFF) * brightness / 255;
  uint8_t b = (color & 0xFF) * brightness / 255;

  for (int i = index; i < index + 3 && i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}


void adjustBrightness() {
  // Map encoder position to brightness (0-255)
  brightness = map(encoderPos, 0, NUM_PIXELS - 1, 0, 255);

  // Set a green color based on the brightness level
  uint8_t red = 0;               // No red
  uint8_t green = 0;    // Full brightness for green
  uint8_t blue = brightness;              // No blue

  // Update the display to show the current brightness level
  pixels.clear();
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Set all pixels to green
  }
  pixels.show();
}

void handleRgbSlider() {
  switch (RgbSliderMode) {
    case 0: RgbSlider(); break;
    case 1: RgbFade(); break;    // Fade between colors
    case 2: RgbPulse(); break;   // Pulsing effect
  }
}

void RgbSlider() {
  // Normalize encoder position to range from 0 to 255
  int pos = encoderPos % 256;

  // Calculate the color based on the encoder position
  if (pos < 85) { // From Red to Green
    red = 255 - (pos * 3);   // Decrease Red
    green = pos * 3;         // Increase Green
    blue = 0;                // Blue is off
  } else if (pos < 170) { // From Green to Blue
    pos -= 85;              // Normalize position to 0-84
    red = 0;                // Red is off
    green = 255 - (pos * 3); // Decrease Green
    blue = pos * 3;         // Increase Blue
  } else { // From Blue to Red
    pos -= 170;             // Normalize position to 0-84
    red = pos * 3;          // Increase Red
    green = 0;              // Green is off
    blue = 255 - (pos * 3); // Decrease Blue
  }

  // Set the pixel colors
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}

void RgbFade() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= animationSpeed) {
    lastUpdateTime = currentTime;

    if (animationStep < 256) {
      red = animationStep;
      green = 255 - animationStep;
      blue = 0;
    } else if (animationStep < 512) {
      red = 255 - (animationStep - 256);
      green = 0;
      blue = animationStep - 256;
    } else if (animationStep < 768) {
      red = 0;
      green = animationStep - 512;
      blue = 255 - (animationStep - 512);
    } else {
      animationStep = 0; // Reset animation
    }

    animationStep++;
    
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
  }
}

void RgbPulse() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= animationSpeed) {
    lastUpdateTime = currentTime;

    float pulseBrightness = (sin(millis() / 500.0) + 1) * 127.5; // Create a pulsing effect
    red = (uint8_t)(pulseBrightness);
    green = (uint8_t)(255 - pulseBrightness);
    blue = (uint8_t)(pulseBrightness / 2);

    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
  }
}

// New Theater Chase Mode
void theaterChase() {
  static int chaseIndex = 0; // Index for the chase effect
  for (int i = 0; i < NUM_PIXELS; i++) {
    if ((i + chaseIndex) % 3 == 0) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // White
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Off
    }
  }

  pixels.show();
  chaseIndex++;
  if (chaseIndex >= 3) {
    chaseIndex = 0; // Reset after completing the chase
  }

  delay(100); // Adjust speed of the chase
}


// Additional variables for remembering game mode
int level = 1;                        // Current level (difficulty)
int currentTarget = 0;                 // Index of the current target LED to select
int targetSequence[16];                // Array to store the sequence of LEDs to remember
bool gameActive = false;               // Whether the game is active
bool waitingForSelection = false;      // Whether waiting for user to select the LED

void rememberingGame() {
  const int waveSpeed = 30; // Adjust speed (lower = faster)
  const int waveBrightness = 255; // Maximum brightness
  const int waveFade = 2; // Fade amount (higher = more gradual)

  if (!gameActive) {
    // Start the game and generate the first sequence
    gameActive = true;
      for (int pos = 0; pos < NUM_PIXELS; pos++) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      int brightness = 0;
      if (abs(i - pos) <= waveFade) {
        brightness = map(abs(i - pos), 0, waveFade, waveBrightness, 0);
      }
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
    }
    pixels.show();
    delay(waveSpeed);
  }
    generateSequence();
    showSequence();
    waitingForSelection = true;
    currentTarget = 0; // Start from the first target
  }
  
  // Continuously update the LED strip to show the encoder's position
  if (waitingForSelection) {
    pixels.clear();
    pixels.setPixelColor(encoderPos, pixels.Color(255, 255, 255)); // Show white LED for encoder position
    pixels.show();
  }
  
  // Check for user selection
  if (waitingForSelection && digitalRead(ENCODER_BTN) == LOW) {
    if (encoderPos == targetSequence[currentTarget]) {
      // Correct selection
      currentTarget++;
      vibrate(); // Provide feedback for correct choice
      
      if (currentTarget >= level) {
        // Level up if all targets are correctly selected
        level++;
  for (int pos = 0; pos < NUM_PIXELS; pos++) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      int brightness = 0;
      if (abs(i - pos) <= waveFade) {
        brightness = map(abs(i - pos), 0, waveFade, waveBrightness, 0);
      }
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
    }
    pixels.show();
    delay(waveSpeed);
  }
        generateSequence();   // Generate new sequence for the next level
        showSequence();       // Show the new sequence
        currentTarget = 0;    // Reset target selection
      }
    } else {
      // Incorrect selection, reset game
      showSequence();
      resetGame();
    }
    delay(200); // Debounce delay
  }
}

void generateSequence() {
  for (int i = 0; i < level; i++) {
    targetSequence[i] = random(NUM_PIXELS); // Random LED position
  }
}

void showSequence() {
  pixels.clear();
  for (int i = 0; i < level; i++) {
    pixels.setPixelColor(targetSequence[i], pixels.Color(0, 255, 0)); // Show green LED
    pixels.show();
    delay(500);                 // Show each LED for half a second
    pixels.setPixelColor(targetSequence[i], 0); // Turn off LED
    pixels.show();
    delay(250);                 // Small pause between LEDs
  }
}

void resetGame() {
  gameActive = false;
  level = 1;
  currentTarget = 0;
  pixels.clear();
  pixels.show();
  // Optionally, flash all LEDs red as an indicator of failure
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < NUM_PIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(255, 0, 0)); // Red flash
    }
    pixels.show();
    delay(200);
    pixels.clear();
    pixels.show();
    delay(200);
  }
}
