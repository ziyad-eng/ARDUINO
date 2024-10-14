#include <Adafruit_Fingerprint.h>
#include <Wire.h>

#define solenoidPin 7
#define blueLED 9
#define greenLED 10
#define buttonPin 4

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int buttonState = 0;
int lastButtonState = 0;
long buttonPressTime = 0;

void setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Fingerprint Lock Test");
  
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("No fingerprint sensor detected!");
    while (1);
  }
  
  digitalWrite(greenLED, HIGH);  // Green LED on at start
  digitalWrite(blueLED, LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && lastButtonState == LOW) {
    buttonPressTime = millis();
  }

  if (buttonState == HIGH && (millis() - buttonPressTime > 4000)) {
    Serial.println("Entering Enroll Mode");
    digitalWrite(blueLED, HIGH);
    digitalWrite(greenLED, LOW);
    
    enrollFingerprint();
    
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, HIGH);
    Serial.println("Enroll complete");
  }

  if (fingerprintMatch()) {
    unlockSolenoid();
  }
  
  lastButtonState = buttonState;
}

void unlockSolenoid() {
  Serial.println("Fingerprint matched, unlocking...");
  digitalWrite(solenoidPin, HIGH);
  delay(3000);  // Keep solenoid open for 3 seconds
  digitalWrite(solenoidPin, LOW);
}

int fingerprintMatch() {
  int p = finger.getImage();
  
  if (p != FINGERPRINT_OK) {
    return 0;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return 0;
  }
  
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return 0;
  }
  
  Serial.println("Fingerprint match found!");
  return 1;
}

void enrollFingerprint() {
  int id = 1;  // You can change this if you want to enroll more fingers
  int p = -1;

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) {
      Serial.println("Place finger on sensor");
    }
  }

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    return;
  }

  Serial.println("Remove finger");
  delay(2000);

  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  Serial.println("Place finger again");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    return;
  }

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint model created!");
  } else {
    Serial.println("Failed to create fingerprint model.");
    return;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint stored!");
  } else {
    Serial.println("Failed to store fingerprint.");
  }
}
