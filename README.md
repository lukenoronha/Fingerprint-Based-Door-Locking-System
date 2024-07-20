# Fingerprint-Based-Door-Locking-System
This project is an Arduino-based fingerprint door lock system that uses the Adafruit Fingerprint Sensor module. It allows users to enroll fingerprints, store them in a database, and verify fingerprints to control a door lock. The system also features a buzzer for feedback during enrollment and access verification.

## Features
* Enroll new fingerprints with a unique ID
* Verify fingerprints to unlock the door
* Reset the fingerprint database
* Buzzer feedback for successful and denied access

## Components
  * Arduino Uno
  * Adafruit Fingerprint Sensor
  * Relay module
  * Buzzer
  * Software Serial for communication
  
## Wiring Instructions
* Fingerprint Sensor
    - VCC to 5V
    - GND to GND
    - RX to Arduino pin 2
    - TX to Arduino pin 3
* Relay Module
    - IN to Arduino pin 12
    - VCC to 5V
    - GND to GND
* Buzzer
    - Positive to Arduino pin 6
    - Negative to GND

## Setup
* Connect the components according to the wiring instructions.
* Open the Arduino IDE and upload the provided code to the Arduino Uno.
* Open the Serial Monitor (set to 9600 baud rate).
* Type 'enroll' in the Serial Monitor to start enrolling fingerprints or 'reset' to clear the fingerprint database.

## Usage
* Enrolling Fingerprints:
  - Type 'enroll' in the Serial Monitor.
  - Follow the on-screen instructions to place and remove your finger from the sensor.
  - Once enrolled, the fingerprint ID is stored.
* Resetting Database:
  - Type 'reset' in the Serial Monitor to clear all stored fingerprints.
* Fingerprint Verification:
  - Place your finger on the sensor.
  - If the fingerprint matches, the door will unlock.
  - If the fingerprint does not match, access is denied and the buzzer sounds.

## Troubleshooting
* Ensure all connections are secure.
* Make sure the fingerprint sensor is correctly wired.
* Verify the correct COM port is selected in the Arduino IDE.
## Potential Use Cases
* Home security systems
* Office access control
* Secure entry for restricted areas
