Fingerprint Lock with Solenoid Control
Project Overview
This project is a ingerprint Lock System that uses an Arduino board to control a solenoid lock. The system includes a fingerprint sensor, an LED for status indication, and a button for enrolling fingerprints. The circuit is designed to activate the solenoid for three seconds upon successful fingerprint recognition.

Components Used
Hardware
- Arduino Board (e.g., Arduino Uno)
- Fingerprint Sensor Module
- Solenoid Lock
- Relay Module (5V)
- NPN Transistor (e.g., 2N2222)
- Resistors (1kΩ, 10kΩ)
- LED (RGB or dual-color)
- Button
- 12V Battery
- Diode (e.g., 1N4001)
- Breadboard and Jumper Wires

 Software
- Arduino IDE

Build Process
1. Design the Circuit: The circuit was designed to connect the fingerprint sensor, solenoid, and LED to the Arduino. I used a relay to control the solenoid activation, which requires higher voltage than the Arduino can supply directly.

2. Assemble Components: I placed all components on a breadboard, ensuring proper connections according to the schematic. The fingerprint sensor connects to the Arduino for data processing, while the relay controls the solenoid.

3. Write the Code: After my original code was deleted, I rewrote the Arduino sketch to implement the fingerprint enrollment and recognition process. I included logic for the LED indicators during enrollment and recognition.

4. Testing: After assembling the circuit and uploading the code, I tested the fingerprint sensor's enrollment and recognition functionality. I adjusted the LED's behavior based on the system state.

Code Overview
The Arduino code handles:
- Fingerprint Enrollment: Pressing the button for four seconds enables enrollment mode, indicated by a blue LED. Once the fingerprint is saved, the LED turns green.
- Fingerprint Recognition: When a fingerprint is recognized, the solenoid activates for three seconds. If unrecognized, the solenoid remains inactive.


Usage Instructions\
1. Wire everything correctly
2. copy my code and upload it to the board
3. connect the 12v bower supply
4. press and hold the button for four seconds to enrol the fingerprint
5. after enrolment you can test out by using the enrolled finger

Collaborators
This project was developed independently; however, I received guidance from Adegoke Akintoye (My Electronics Teacher) mentor
