#include <SoftwareSerial.h>

const int buttonC1 = 8; // Input D1
const int buttonC2 = 9; // Input E1
const int buttonC3 = 10; // Input F1
const int buttonC4 = 11; // Input D2
const int buttonC5 = 12; // Input E2
const int buttonC6 = 13; // Input F2

const int ledA4 = 2; // Output D1 (for any Arduino)
const int ledA5 = 3; // Output E1 (for any Arduino)
const int ledA6 = 4; // Output F1 (for any Arduino)
const int ledB4 = 5; // Output D2 (for any Arduino)
const int ledB5 = 6; // Output E2 (for any Arduino)
const int ledB6 = 7; // Output F2 (for any Arduino)

SoftwareSerial serial(0, 1); // RX, TX for RS-485

const unsigned long DEBOUNCE_DELAY = 50;    // Debounce time in milliseconds
const unsigned long TIME_SLOT_DURATION = 30; // Duration of each time slot in milliseconds
const unsigned long CYCLE_DURATION = 90;     // Total time for all 3 Arduinos


unsigned long lastDebounceTime[6] = {0, 0, 0, 0, 0, 0};  // Last time buttons were pressed

void sendButtonState(String switchName, int state);

void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging

    // Set button pins as input with pull-up resistors
    pinMode(buttonC1, INPUT_PULLUP);
    pinMode(buttonC2, INPUT_PULLUP);
    pinMode(buttonC3, INPUT_PULLUP);
    pinMode(buttonC4, INPUT_PULLUP);
    pinMode(buttonC5, INPUT_PULLUP);
    pinMode(buttonC6, INPUT_PULLUP);

    // Set LED pins as output
    pinMode(ledA4, OUTPUT);
    pinMode(ledA5, OUTPUT);
    pinMode(ledA6, OUTPUT);
    pinMode(ledB4, OUTPUT);
    pinMode(ledB5, OUTPUT);
    pinMode(ledB6, OUTPUT);

    // Turn off all LEDs by default
    digitalWrite(ledA4, LOW);
    digitalWrite(ledA5, LOW);
    digitalWrite(ledA6, LOW);
    digitalWrite(ledB4, LOW);
    digitalWrite(ledB5, LOW);
    digitalWrite(ledB6, LOW);
}

// Store previous button states
int prevState1 = LOW, prevState2 = LOW, prevState3 = LOW;
int prevState4 = LOW, prevState5 = LOW, prevState6 = LOW;

void loop() {
    unsigned long currentMillis = millis();
    // Arduino B's transmission window (30-59ms of every 90ms cycle)
    // First Arduino (A) uses 0-29ms
    // Second Arduino (B) uses 30-59ms
    // Third Arduino (C) uses 60-89ms
    // Calculate time slot - Arduino C gets third slot (60-89ms of every 90ms cycle)
    if ((currentMillis % CYCLE_DURATION) >= (TIME_SLOT_DURATION * 2)) {
        // Read current button states
        int stateC1 = digitalRead(buttonC1);
        int stateC2 = digitalRead(buttonC2);
        int stateC3 = digitalRead(buttonC3);
        int stateC4 = digitalRead(buttonC4);
        int stateC5 = digitalRead(buttonC5);
        int stateC6 = digitalRead(buttonC6);

        // Check for state changes with debouncing
        if (stateC1 != prevState1 && (currentMillis - lastDebounceTime[0]) > DEBOUNCE_DELAY) {
            sendButtonState("C1", stateC1);
            prevState1 = stateC1;
            lastDebounceTime[0] = currentMillis;
        }
        if (stateC2 != prevState2 && (currentMillis - lastDebounceTime[1]) > DEBOUNCE_DELAY) {
            sendButtonState("C2", stateC2);
            prevState2 = stateC2;
            lastDebounceTime[1] = currentMillis;
        }
        if (stateC3 != prevState3 && (currentMillis - lastDebounceTime[2]) > DEBOUNCE_DELAY) {
            sendButtonState("C3", stateC3);
            prevState3 = stateC3;
            lastDebounceTime[2] = currentMillis;
        }   
        if (stateC4 != prevState4 && (currentMillis - lastDebounceTime[3]) > DEBOUNCE_DELAY) {
            sendButtonState("C4", stateC4);
            prevState4 = stateC4;
            lastDebounceTime[3] = currentMillis;
        }        
        if (stateC5 != prevState5 && (currentMillis - lastDebounceTime[4]) > DEBOUNCE_DELAY) {
            sendButtonState("C5", stateC5);
            prevState5 = stateC5;
            lastDebounceTime[4] = currentMillis;
        }        
        if (stateC6 != prevState6 && (currentMillis - lastDebounceTime[5]) > DEBOUNCE_DELAY) {
            sendButtonState("C6", stateC6);
            prevState6 = stateC6;
            lastDebounceTime[5] = currentMillis;
        }
    }

    // Check for incoming data from other Arduinos
    if (Serial.available()) {
        String message = Serial.readStringUntil('\n');

        // Parse the message
        String switchName = message.substring(0, message.indexOf(','));
        int state = message.substring(message.indexOf(',') + 1).toInt();

        // Control LEDs based on switch name
        if (switchName == "A4") {
            digitalWrite(ledA4, state);
        } else if (switchName == "A5") {
            digitalWrite(ledA5, state);
        } else if (switchName == "A6") {
            digitalWrite(ledA6, state);
        } else if (switchName == "B4") {
            digitalWrite(ledB4, state);
        } else if (switchName == "B5") {
            digitalWrite(ledB5, state);
        } else if (switchName == "B6") {
            digitalWrite(ledB6, state);
        }
    }

    delay(100); // Adjust delay as needed
}

// Function to send button state as a string
void sendButtonState(String switchName, int state) {
    String message = switchName + "," + String(state);
    Serial.println(message); // Send the message
}
