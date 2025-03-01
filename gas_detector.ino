#include <LiquidCrystal.h>

// Pin definitions
const int buzzer = 13;       // Buzzer pin
const int sensorPin = A0;    // MQ2 sensor pin (Analog input)
const int redLED = 7;        // Red LED (Alert)
const int greenLED = 6;      // Green LED (Safe)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // LCD pin connections

// Threshold for sensor reading (adjust based on testing)
int sensorThreshold = 400;  

// Variables to track last alert time
unsigned long lastAlertTime = 0;

// Initialize LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    pinMode(buzzer, OUTPUT);
    pinMode(sensorPin, INPUT);
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    
    Serial.begin(9600); // Initialize Serial Monitor

    lcd.begin(16, 2); // Set up LCD display

    // Self-test sequence
    lcd.print("Starting Self-Test");
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, HIGH);
    tone(buzzer, 1000, 500);
    delay(1000);
    noTone(buzzer);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    lcd.clear();

    lcd.print("System Ready");
    delay(2000);
    lcd.clear();
}

void loop() {
    int sensorValue = analogRead(sensorPin);

    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    lcd.setCursor(0, 0);
    lcd.print("Sensor: ");
    lcd.print(sensorValue);

    if (sensorValue > sensorThreshold) {
        // Alert: Gas level exceeded
        tone(buzzer, 1000);
        digitalWrite(redLED, HIGH);
        digitalWrite(greenLED, LOW);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ALERT! GAS HIGH");
        lcd.setCursor(0, 1);
        lcd.print("Evacuate NOW!");
        
        lastAlertTime = millis(); // Store alert time
        delay(2000);
    } else {
        // Safe state
        noTone(buzzer);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ALL SAFE");
        lcd.setCursor(0, 1);
        lcd.print("GAS LEVEL OK");
        delay(2000);
    }

    // Show last alert time if any
    if (lastAlertTime > 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Last Alert:");
        lcd.setCursor(0, 1);
        lcd.print((millis() - lastAlertTime) / 1000);
        lcd.print(" sec ago");
        delay(2000);
    }
}
