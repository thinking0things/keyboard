#include <BleKeyboard.h>    // Include the Bluetooth Keyboard library

// Create a BluetoothKeyboard object
BleKeyboard bleKeyboard;

// Define the pins for the letter keys
const int hPin = 27;  // Pin for ' ' -> GPIO 32
const int ePin = 26;  // Pin for 'e' -> GPIO 33
const int lPin = 25;  // Pin for 't' -> GPIO 25
const int oPin = 33;  // Pin for 'a' -> GPIO 26
const int bPin = 32;  // Pin for 'o' -> GPIO 27

// Define debounce settings
const unsigned long debounceWindow = 200;  // Time window in milliseconds

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Begin Bluetooth Keyboard
  bleKeyboard.begin();

  Serial.println("Bluetooth Keyboard is ready to pair");

  // Configure the letter key pins
  pinMode(hPin, INPUT_PULLUP);
  pinMode(ePin, INPUT_PULLUP);
  pinMode(lPin, INPUT_PULLUP);
  pinMode(oPin, INPUT_PULLUP);
  pinMode(bPin, INPUT_PULLUP);
}

void loop() {
  if (bleKeyboard.isConnected()) {
    // Wait for the first button press
    if (digitalRead(hPin) == HIGH && digitalRead(ePin) == HIGH && digitalRead(lPin) == HIGH && digitalRead(oPin) == HIGH && digitalRead(bPin) == HIGH) {
      return;
    }

    // Start the 200 ms window
    unsigned long windowStartTime = millis();
    bool hPressed = false, ePressed = false, lPressed = false, oPressed = false, bPressed = false;

    while (millis() - windowStartTime < debounceWindow) {
      // Read the state of each button
      hPressed |= (digitalRead(hPin) == LOW);
      ePressed |= (digitalRead(ePin) == LOW);
      lPressed |= (digitalRead(lPin) == LOW);
      oPressed |= (digitalRead(oPin) == LOW);
      bPressed |= (digitalRead(bPin) == LOW);
    }

    // Create a binary pattern based on the button states
    Serial.print("Binary vector: ");
    Serial.print(hPressed);
    Serial.print(ePressed);
    Serial.print(lPressed);
    Serial.print(oPressed);
    Serial.println(bPressed);
    int pattern = (hPressed << 4) | (ePressed << 3) | (lPressed << 2) | (oPressed << 1) | bPressed;

    // Map the pattern to a character and send via Bluetooth
    char outputChar = '\0';
    switch (pattern) {
      case 0b10000: outputChar = ' '; break;
      case 0b01000: outputChar = 'e'; break;
      case 0b00100: outputChar = 't'; break;
      case 0b00010: outputChar = 'a'; break;
      case 0b00001: outputChar = 'o'; break;
      case 0b11000: outputChar = 'i'; break;
      case 0b10100: outputChar = 'n'; break;
      case 0b10010: outputChar = 's'; break;
      case 0b10001: outputChar = 'h'; break;
      case 0b01100: outputChar = 'r'; break;
      case 0b01010: outputChar = 'd'; break;
      case 0b01001: outputChar = 'l'; break;
      case 0b00110: outputChar = 'c'; break;
      case 0b00101: outputChar = 'u'; break;
      case 0b00011: outputChar = 'm'; break;
      case 0b11100: outputChar = 'w'; break;
      case 0b11010: outputChar = 'f'; break;
      case 0b11001: outputChar = 'g'; break;
      case 0b10110: outputChar = 'y'; break;
      case 0b10101: outputChar = 'p'; break;
      case 0b10011: outputChar = 'b'; break;
      case 0b01110: outputChar = 'v'; break;
      case 0b01101: outputChar = 'k'; break;
      case 0b01011: outputChar = 'j'; break;
      case 0b00111: outputChar = KEY_BACKSPACE; break;
      case 0b11110: outputChar = 'x'; break;
      case 0b11101: outputChar = 'q'; break;
      case 0b11111: outputChar = '\n'; break;
      case 0b10111: outputChar = '.'; break;
      case 0b11011: outputChar = ','; break;
      case 0b01111: outputChar = '?'; break;
      default: break;
    }

    if (outputChar != '\0') {
      if (outputChar == KEY_BACKSPACE) {
        bleKeyboard.press(KEY_BACKSPACE);
        delay(300);
        bleKeyboard.release(KEY_BACKSPACE);
      } else {
        bleKeyboard.print(outputChar);
      }
    }
  } else {
    Serial.println("Keyboard not connected");
    delay(1000);  // Check every second if the Bluetooth keyboard is connected
  }
}