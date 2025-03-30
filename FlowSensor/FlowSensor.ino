#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 2  // Water flow sensor connected to pin 2 (Interrupt pin)

volatile int pulseCount;  // Pulse counter
float flowRate;           // Flow rate in L/min
float totalLiters;        // Total water passed in Liters
unsigned long lastTime;   // Timer for calculating flow rate

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C address may be 0x27 or 0x3F

// Interrupt service routine for flow sensor
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
  
  lcd.init();   
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Water Flow Meter");
  
  pulseCount = 0;
  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastTime;

  if (elapsedTime >= 1000) {  
    detachInterrupt(FLOW_SENSOR_PIN);  
    
    // Convert pulses to flow current rate
    flowRate = (pulseCount / 7.5);  //(7.5 pulses = 1 L/min)
    totalLiters += (flowRate / 60.0);  // Convert L/min to total liters of floowed

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rate: ");
    lcd.print(flowRate);
    lcd.print(" L/min");

    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(totalLiters);
    lcd.print(" L");

    pulseCount = 0;  // Reset pulse count
    lastTime = currentTime;
    
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
  }
}
