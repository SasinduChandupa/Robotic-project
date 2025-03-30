#include <Keypad.h>
#include <Servo.h>

#define SERVO_PIN 10  // Servo motor connected to pin 10
Servo gateServo;

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};  
byte colPins[COLS] = {6, 7, 8, 9};  
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void openGate(int delayTime) {
  gateServo.write(90); // Open gate
  Serial.print("Gate Open for ");
  Serial.print(delayTime);
  Serial.println(" minutes");
  
  unsigned long closeTime = millis() + (delayTime * 60000); // Convert minutes to Milliseconds
  
  while (millis() < closeTime) {
    char key = keypad.getKey();
    if (key == 'A') {
      Serial.println("Emergency Close!");
      break;  // exit loop for gate close
    }
  }
  
  gateServo.write(0); // finally Close gate
  Serial.println("Gate Closed");
}

void setup() {
  Serial.begin(9600);
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Ensure the gate is initially closed
}

void loop() {
  char key = keypad.getKey();
  
  if (key >= '1' && key <= '9') {
    int delayTime = key - '0';  // Convert char to integer (e.g., '3' -> 3)
    openGate(delayTime);
  }
  
  if (key == 'A') {
    Serial.println("Emergency Close Activated");
    gateServo.write(0); // Close the gate immediately
  }
}
