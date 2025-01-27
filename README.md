# **RS485_Multi_Arduino_Communication**
Communication between 3 Arduinos using the RS485 module

---

## **Project Overview**
This project demonstrates a multi-Arduino communication system using RS485 protocol, allowing three Arduinos to communicate with each other in a multi-master setup. Each Arduino has its own set of LEDs and buttons, and is able control specific LEDs of the other Arduinos. RS485 ensures reliable long-distance communication with reduced noise interference and helps with the multi-master setup.

---

## **Features**
- Multi-Arduino Communication: Enables communication between three Arduinos using RS485.
- Master-Slave Hybrid: All Arduinos act as both masters and slaves, transmitting and receiving data.
- LED Control: Each Arduino has 6 LEDs, and specific LEDs can be controlled remotely by other Arduinos.
- Button Input: 6 buttons on each Arduino, with state changes transmitted to other Arduinos.
- Debouncing: Ensures stable input detection for buttons.
- Time-Slot Based Communication: Each Arduino has a designated communication window within a shared cycle to avoid collisions.

---

## **Components**
- **Arduino**: 3 x Arduino boards.
- **RS485 Modules**: 3 x RS485-to-TTL modules.
- Jumper wires, breadboard, 18 x LEDs, 18 x Push buttons, Resistors.

---

## **Hardware Connections**
### **Arduino to RS485 Module**
- VCC → Arduino 5V.
- GND → Arduino GND.
- TXD → Arduino TX pin.
- RXD → Arduino RX pin.
  
### **RS485 Module**
- Connect all modules A pins together.
- Connect all modules B pins together.
- Place a 120Ω termination resistor between A and B at both ends of the RS485 bus for proper termination and noise reduction.

### **LEDs**
- Connect one leg of each LED to the designated output pins.
- The other leg connects to GND through a 220Ω resistor.

### **Buttons**
- Connect one leg of each button to the designated input pins.
- The other leg connects to GND. (used internal pull-up resistors)

---

## **Software Setup**
1. Install **Arduino IDE**.
2. Install **SoftwareSerial library**.

---

