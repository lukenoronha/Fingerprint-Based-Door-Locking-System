#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Define pins for relay, buzzer, and fingerprint sensor communication
#define RELAY_PIN 12
#define BUZZER_PIN 6
#define FINGERPRINT_RX_PIN 2
#define FINGERPRINT_TX_PIN 3

// Initialize software serial for fingerprint sensor communication
SoftwareSerial mySerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int enrollID = 1; // Initial ID for enrolling fingerprints

void setup() {
  // Set pin modes
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Set initial state for relay and buzzer
  digitalWrite(RELAY_PIN, HIGH); // Door locked
  digitalWrite(BUZZER_PIN, LOW); // Buzzer off

  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial); // Wait for serial to initialize
  Serial.println("Fingerprint sensor setup");

  // Initialize the fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1); // Halt the program if sensor is not found
  }

  // Display setup options
  Serial.println("Type 'enroll' to start enrollment process or 'reset' to reset the database.");
}

void loop() {
  // Check for user commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equals("enroll")) {
      enrollFingerprint();
    } else if (command.equals("reset")) {
      resetFingerprintDatabase();
    }
  }

  // Perform fingerprint verification
  int fingerprintResult = fingerprintVerification();
  if (fingerprintResult == FINGERPRINT_OK) {
    Serial.println("Fingerprint matched, unlocking door.");
    unlockDoor();
  } else if (fingerprintResult == FINGERPRINT_NOTFOUND) {
    Serial.println("Fingerprint mismatch, access denied.");
    deniedAccess();
  }

  delay(1000); // Small delay to avoid continuous scanning
}

void enrollFingerprint() {
  Serial.println("Enrolling ID #" + String(enrollID));
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error: Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Error: Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
    delay(500);
  }

  // Convert the image to a fingerprint template
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Error: Template conversion error");
    return;
  }
  Serial.println("Image converted");

  Serial.println("Remove finger");
  delay(2000);
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  Serial.println("Place same finger again");
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error: Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Error: Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
    delay(500);
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    Serial.println("Error: Template conversion error");
    return;
  }

  // Create a model
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error: Communication error");
    return;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Error: Fingerprints did not match");
    return;
  } else {
    Serial.println("Unknown error");
    return;
  }

  p = finger.storeModel(enrollID);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    enrollID++; // Increment enrollID for the next enrollment
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error: Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Error: Could not store in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error: Error writing to flash");
  } else {
    Serial.println("Unknown error");
  }
}

void resetFingerprintDatabase() {
  Serial.println("Resetting fingerprint database...");
  finger.emptyDatabase();
  Serial.println("Database reset complete.");
  enrollID = 1; // Reset enrollID to 1 after database reset
}

int fingerprintVerification() {
  Serial.println("Waiting for valid finger...");
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("No valid finger detected.");
    return FINGERPRINT_NOFINGER;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("Error in converting image.");
    return FINGERPRINT_IMAGEFAIL;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Fingerprint not found.");
    return FINGERPRINT_NOTFOUND;
  }

  Serial.println("Fingerprint found, ID #" + String(finger.fingerID));
  return FINGERPRINT_OK;
}

void unlockDoor() {
  Serial.println("Unlocking door...");
  digitalWrite(RELAY_PIN, LOW); // Unlock door
  buzz(2, 250); // Short beep to indicate door unlock
  delay(5000); // Keep door unlocked for 5 seconds
  digitalWrite(RELAY_PIN, HIGH); // Lock door
  Serial.println("Door locked.");
}

void deniedAccess() {
  Serial.println("Access denied.");
  buzz(1, 1000); // Sound buzzer for 1 second
}

void buzz(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100); // Small delay between buzzes
  }
}
