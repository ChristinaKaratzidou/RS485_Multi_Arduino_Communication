#include <SoftwareSerial.h>

const int buttonA1 = 8; // Input A
const int buttonA2 = 9; // Input B
const int buttonA3 = 10; // Input C
const int buttonA4 = 11; // Input A1
const int buttonA5 = 12; // Input B1
const int buttonA6 = 13; // Input C1

const int ledB1 = 2; // Output A (for any Arduino)
const int ledB2 = 3; // Output B (for any Arduino)
const int ledB3 = 4; // Output C (for any Arduino)
const int ledC1 = 5; // Output A1 (for any Arduino)
const int ledC2 = 6; // Output B1 (for any Arduino)
const int ledC3 = 7; // Output C1 (for any Arduino)

SoftwareSerial serial(0, 1); // RX, TX for RS-485


const unsigned long DEBOUNCE_DELAY = 50;    // Debounce time in milliseconds
const unsigned long TIME_SLOT_DURATION = 30; // Duration of each time slot in milliseconds
const unsigned long CYCLE_DURATION = 90;     // Total time for all 3 Arduinos (3 * 30ms)


unsigned long lastDebounceTime[6] = {0, 0, 0, 0, 0, 0};  // Last time buttons were pressed

void sendButtonState(String switchName, int state);

void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging

    // Set button pins as input with pull-up resistors
    pinMode(buttonA1, INPUT_PULLUP);
    pinMode(buttonA2, INPUT_PULLUP);
    pinMode(buttonA3, INPUT_PULLUP);
    pinMode(buttonA4, INPUT_PULLUP);
    pinMode(buttonA5, INPUT_PULLUP);
    pinMode(buttonA6, INPUT_PULLUP);

    // Set LED pins as output
    pinMode(ledB1, OUTPUT);
    pinMode(ledB2, OUTPUT);
    pinMode(ledB3, OUTPUT);
    pinMode(ledC1, OUTPUT);
    pinMode(ledC2, OUTPUT);
    pinMode(ledC3, OUTPUT);

    // Turn off all LEDs by default
    digitalWrite(ledB1, LOW);
    digitalWrite(ledB2, LOW);
    digitalWrite(ledB3, LOW);
    digitalWrite(ledC1, LOW);
    digitalWrite(ledC2, LOW);
    digitalWrite(ledC3, LOW);
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
    // Calculate time slot - Arduino A gets first slot (0-29ms of every 90ms cycle)
    if ((currentMillis % CYCLE_DURATION) < TIME_SLOT_DURATION) {
        // Read current button states
        int stateA1 = digitalRead(buttonA1);
        int stateA2 = digitalRead(buttonA2);
        int stateA3 = digitalRead(buttonA3);
        int stateA4 = digitalRead(buttonA4);
        int stateA5 = digitalRead(buttonA5);
        int stateA6 = digitalRead(buttonA6);

        // Check for state changes with debouncing
        if (stateA1 != prevState1 && (currentMillis - lastDebounceTime[0]) > DEBOUNCE_DELAY) {
            sendButtonState("A1", stateA1);
            prevState1 = stateA1;
            lastDebounceTime[0] = currentMillis;
        }
        if (stateA2 != prevState2 && (currentMillis - lastDebounceTime[1]) > DEBOUNCE_DELAY) {
            sendButtonState("A2", stateA2);
            prevState2 = stateA2;
            lastDebounceTime[1] = currentMillis;
        }
        if (stateA3 != prevState3 && (currentMillis - lastDebounceTime[2]) > DEBOUNCE_DELAY) {
            sendButtonState("A3", stateA3);
            prevState3 = stateA3;
            lastDebounceTime[2] = currentMillis;
        }
        if (stateA4 != prevState4 && (currentMillis - lastDebounceTime[3]) > DEBOUNCE_DELAY) {
            sendButtonState("A4", stateA4);
            prevState4 = stateA4;
            lastDebounceTime[3] = currentMillis;
        }
        if (stateA5 != prevState5 && (currentMillis - lastDebounceTime[4]) > DEBOUNCE_DELAY) {
            sendButtonState("A5", stateA5);
            prevState5 = stateA5;
            lastDebounceTime[4] = currentMillis;
        }
        if (stateA6 != prevState6 && (currentMillis - lastDebounceTime[5]) > DEBOUNCE_DELAY) {
            sendButtonState("A6", stateA6);
            prevState6 = stateA6;
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
        if (switchName == "B1") {
            digitalWrite(ledB1, state);
        } else if (switchName == "B2") {
            digitalWrite(ledB2, state);
        } else if (switchName == "B3") {
            digitalWrite(ledB3, state);
        } else if (switchName == "C1") {
            digitalWrite(ledC1, state);
        } else if (switchName == "C2") {
            digitalWrite(ledC2, state);
        } else if (switchName == "C3") {
            digitalWrite(ledC3, state);
        }
    }

    delay(1); // 
}

// Function to send button state as a string
void sendButtonState(String switchName, int state) {
    String message = switchName + "," + String(state);
    Serial.println(message); // Send the message
 
}
