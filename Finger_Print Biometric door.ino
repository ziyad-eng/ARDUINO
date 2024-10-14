#include <Adafruit_Fingerprint.h> // Library for fingerprint sensor magic
#include <Wire.h> // I2C communication wizardry

#define solenoidPin 7  // Pin to control the solenoid lock
#define blueLED 9      // The blue LED for feedback
#define greenLED 10    // The green LED for success
#define buttonPin 4    // The button that starts it all

// Setting up the fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int buttonState = 0; // Current button state
int lastButtonState = 0; // Previous button state
long buttonPressTime = 0; // Timer for button press

void setup() {
  pinMode(solenoidPin, OUTPUT); // Prepare the solenoid pin
  pinMode(blueLED, OUTPUT); // Blue LED setup
  pinMode(greenLED, OUTPUT); // Green LED setup
  pinMode(buttonPin, INPUT); // Button pin setup
  
  Serial.begin(9600); // Start the serial communication
  Serial.println("🔑 Fingerprint Lock Test Initialized!");

  finger.begin(57600); // Fire up the fingerprint sensor
  
  // Check if the fingerprint sensor is ready
  if (finger.verifyPassword())
  {Serial.println(" Found fingerprint sensor!")}
  else {Serial.println(" No fingerprint sensor detected! Better luck next time!");
    while (1); // Stop everything if the sensor is not found
       }
  
  digitalWrite(greenLED, HIGH); // Green LED on at start
  digitalWrite(blueLED, LOW); // Blue LED off initially
       }

void loop() {
  buttonState = digitalRead(buttonPin); // Read the button state
  // Check if the button was just pressed
  if (buttonState == HIGH && lastButtonState == LOW) {
    buttonPressTime = millis(); // Start the timer
  }

  // If the button is pressed for more than 4 seconds, enter enrollment mode
  if (buttonState == HIGH && (millis() - buttonPressTime > 4000)) {
    Serial.println("🛠️ Entering Enroll Mode...");
    digitalWrite(blueLED, HIGH); // Turn on the blue LED
    digitalWrite(greenLED, LOW); // Turn off the green LED
    
    enrollFingerprint(); // Call the function to enroll fingerprint
    
    digitalWrite(blueLED, LOW); // Turn off the blue LED
    digitalWrite(greenLED, HIGH); // Turn on the green LED
    Serial.println("✅ Enroll complete!");
  }

  // Check for a matching fingerprint
  if (fingerprintMatch()) {
    unlockSolenoid(); // Unlock the solenoid if the fingerprint matches
  }
  
  lastButtonState = buttonState; // Save the button state for next loop
}

// Unlock the solenoid lock
void unlockSolenoid() {
  Serial.println("🔓 Fingerprint matched! Unlocking...");
  digitalWrite(solenoidPin, HIGH); // Activate solenoid
  delay(3000); // Keep it unlocked for 3 seconds
  digitalWrite(solenoidPin, LOW); // Lock it again
}

// Check if the fingerprint matches
int fingerprintMatch() {
  int p = finger.getImage(); // Get the fingerprint image
  
  if (p != FINGERPRINT_OK) {
    return 0; // No image found
  }

  p = finger.image2Tz(); // Convert the image to template
  if (p != FINGERPRINT_OK) {
    return 0; // Failed to convert
  }
  
  p = finger.fingerFastSearch(); // Search for the fingerprint
  if (p != FINGERPRINT_OK) {
    return 0; // No match found
  }
  
  Serial.println("✅ Fingerprint match found!");
  return 1; // Match found
}

// Enroll a new fingerprint
void enrollFingerprint() {
  int id = 1; // ID for the fingerprint (you can change this)
  int p = -1;

  // Wait for a finger to be placed on the sensor
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) {
      Serial.println("Place your finger on the sensor.");
    }
  }

  p = finger.image2Tz(1); // Convert the first image to ID
  if (p != FINGERPRINT_OK) {
    return; // Exit if failed
  }

  Serial.println(" Remove your finger.");
  delay(2000); // Wait for 2 seconds

  // Wait for the finger to be removed
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  Serial.println("🖐️ Place your finger again...");
  
  // Wait for the second image
  while (p != FINGERPRINT_OK) 
  {p = finger.getImage();}

  p = finger.image2Tz(2); // Convert the second image
  if (p != FINGERPRINT_OK) 
  {return; // Exit if failed}

  p = finger.createModel(); // Create the fingerprintID
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint model created!");
  } else {
    Serial.println("Failed to create fingerprint model.");
    return; // Exit it if it failed
  }

  p = finger.storeModel(id); // Store the  ID
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint stored!");
  } else {
    Serial.println("Failed to store fingerprint.");
  }
}
