#include <SoftwareSerial.h>

const int buttonB1 = 8; // Input D
const int buttonB2 = 9; // Input E
const int buttonB3 = 10; // Input F
const int buttonB4 = 11; // Input D1
const int buttonB5 = 12; // Input E1
const int buttonB6 = 13; // Input F1

const int ledA1 = 2; // Output D (for any Arduino)
const int ledA2 = 3; // Output E (for any Arduino)
const int ledA3 = 4; // Output F (for any Arduino)
const int ledC4 = 5; // Output D1 (for any Arduino)
const int ledC5 = 6; // Output E1 (for any Arduino)
const int ledC6 = 7; // Output F1 (for any Arduino)

SoftwareSerial serial(0,  1); // RX, TX for RS-485

const unsigned long DEBOUNCE_DELAY = 50;    // Debounce time in milliseconds
const unsigned long TIME_SLOT_DURATION = 30; // Duration of each time slot in milliseconds
const unsigned long CYCLE_DURATION = 90;     // Total time for all 3 Arduinos


unsigned long lastDebounceTime[6] = {0, 0, 0, 0, 0, 0};  // Last time buttons were pressed

void sendButtonState(String switchName, int state);

void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging

    // Set button pins as input with pull-up resistors
    pinMode(buttonB1, INPUT_PULLUP);
    pinMode(buttonB2, INPUT_PULLUP);
    pinMode(buttonB3, INPUT_PULLUP);
    pinMode(buttonB4, INPUT_PULLUP);
    pinMode(buttonB5, INPUT_PULLUP);
    pinMode(buttonB6, INPUT_PULLUP);

    // Set LED pins as output
    pinMode(ledA1, OUTPUT);
    pinMode(ledA2, OUTPUT);
    pinMode(ledA3, OUTPUT);
    pinMode(ledC4, OUTPUT);
    pinMode(ledC5, OUTPUT);
    pinMode(ledC6, OUTPUT);

    // Turn off all LEDs by default
    digitalWrite(ledA1, LOW);
    digitalWrite(ledA2, LOW);
    digitalWrite(ledA3, LOW);
    digitalWrite(ledC4, LOW);
    digitalWrite(ledC5, LOW);
    digitalWrite(ledC6, LOW);
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
    // Calculate time slot - Arduino B gets second slot (30-59ms of every 90ms cycle)
    if (((currentMillis % CYCLE_DURATION) >= TIME_SLOT_DURATION) && 
        ((currentMillis % CYCLE_DURATION) < (TIME_SLOT_DURATION * 2))) {
        // Read current button states
        int stateB1 = digitalRead(buttonB1);
        int stateB2 = digitalRead(buttonB2);
        int stateB3 = digitalRead(buttonB3);
        int stateB4 = digitalRead(buttonB4);
        int stateB5 = digitalRead(buttonB5);
        int stateB6 = digitalRead(buttonB6);

        // Check for state changes with debouncing
        if (stateB1 != prevState1 && (currentMillis - lastDebounceTime[0]) > DEBOUNCE_DELAY) {
            sendButtonState("B1", stateB1);
            prevState1 = stateB1;
            lastDebounceTime[0] = currentMillis;
        }
        if (stateB2 != prevState2 && (currentMillis - lastDebounceTime[1]) > DEBOUNCE_DELAY) {
            sendButtonState("B2", stateB2);
            prevState2 = stateB2;
            lastDebounceTime[1] = currentMillis;
        }
        if (stateB3 != prevState3 && (currentMillis - lastDebounceTime[2]) > DEBOUNCE_DELAY) {
            sendButtonState("B3", stateB3);
            prevState3 = stateB3;
            lastDebounceTime[2] = currentMillis;
        }
        if (stateB4 != prevState4 && (currentMillis - lastDebounceTime[3]) > DEBOUNCE_DELAY) {
            sendButtonState("B4", stateB4);
            prevState4 = stateB4;
            lastDebounceTime[3] = currentMillis;
        }
        if (stateB5 != prevState5 && (currentMillis - lastDebounceTime[4]) > DEBOUNCE_DELAY) {
            sendButtonState("B5", stateB5);
            prevState5 = stateB5;
            lastDebounceTime[4] = currentMillis;
         }
        if (stateB6 != prevState6 && (currentMillis - lastDebounceTime[5]) > DEBOUNCE_DELAY) {
            sendButtonState("B6", stateB6);
            prevState6 = stateB6;
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
        if (switchName == "A1") {
            digitalWrite(ledA1, state);
        } else if (switchName == "A2") {
            digitalWrite(ledA2, state);
        } else if (switchName == "A3") {
            digitalWrite(ledA3, state);
        } else if (switchName == "C4") {
            digitalWrite(ledC4, state);
        } else if (switchName == "C5") {
            digitalWrite(ledC5, state);
        } else if (switchName == "C6") {
            digitalWrite(ledC6, state);
        }
    }

    delay(1); // Adjust delay as needed
}

// Function to send button state as a string
void sendButtonState(String switchName, int state) {
    String message = switchName + "," + String(state);
    Serial.println(message); // Send the message
}
