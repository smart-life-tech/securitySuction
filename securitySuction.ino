/*https://wokwi.com/projects/397220140183087105

  It is for a security device that creates suction on a surface, the steps would be as follows.

  1.	ON/ OFF Switch – when switched on…
  2.	User will tap their fob on the RFID Module
  3.	When On and When RFID Tapped and held for a Second the System is
  activated – USE RGB LED To show system is activated – Green is system
  active -  Amber is system on but inactive)
  4.	Activate micro–Servo Motor: The microcontroller sends a signal to
  activate the Servo motor, which engages the physical lock mechanism
  to secure the device. Rotate 180 degrees clockwise.
  5.	Prime Suction Motor (24V DC Motor):  -- Simultaneously, the vacuum
  pump is activated to start creating a base level of suction, securing
  the device to the surface. (This needs to reach -0.6 bars of pressure)
  -- Pressure Sensor (e.g., NXP MPX5050):
  6.	This sensor can be connected to an input on the Arduino to monitor
  the vacuum pressure continuously.
  7.	Use of a 24-33V Lithium – ion battery - Power management circuit
  to safely power the Arduino and other components, likely requiring a
  voltage regulator or a DC-DC converter to step down the voltage to 5V or
  3.3V as required by the Arduino and sensors.
  8.	Activate Sensors: With the system now active, the shock and tamper
  sensors are primed and start monitoring for any movements or
  signs of tampering. This could be hall sensor or vibration sensor
  – to detect if someone is tampering with the device…
  9.	If the tampering sensors are activated a buzzer alarm should be
  activated and the Suction motor should be activated on full speed/
  power for a duration of 10 min every time further tampering is
  detected it will reactivate to keep pressure.
  10.	 When the user taps the RFID again and holds it for 2 Second the
  LED will change from Green to Amber and deactivate the system
  (sensors etc) and then the servo will move the opposite direction
  to open the lock mechanism – rotates anticlockwise 180 degrees to
  original position
  For the tampering sensor in the simulated code,
  I used a simple switch connected to pin 2 (SWITCH_PIN).
  This switch simulates the tampering sensor detecting any
  movement or tampering with the device. When the switch is
  triggered (pressed),
  it activates the alarm as per the provided requirements.

  Since this simulator does not have an RFID tag, we can indeed
  use a button to simulate the RFID tapping action.
  You can connect the button to any digital pin of the Arduino,
  and in the code, we'll monitor that pin for button presses to
  activate or deactivate the system.
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define tamper 11
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
#define RFID_LED_PIN 7
#define SERVO_PIN 5
#define SUCTION_PIN 6
#define PRESSURE_SENSOR_PIN A0
#define BUZZER_PIN 4
#define SWITCH_PIN 2// tamper sensor
#define rfid_simulationPin 3
#define SUCTION_THRESHOLD 3.6 // Threshold pressure in bars

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Servo lockServo;

bool systemActivated = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lockServo.attach(SERVO_PIN);
  pinMode(RFID_LED_PIN, OUTPUT);
  pinMode(SUCTION_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(rfid_simulationPin, INPUT_PULLUP);
  pinMode(tamper, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(SWITCH_PIN) == LOW) {
    //if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (digitalRead(rfid_simulationPin) == LOW) {
      if (!systemActivated) {
        activateSystem();
      } else {
        deactivateSystem();
      }
    }
  }else{
     deactivateSystem();
  }

  if (systemActivated) {
    // Check pressure sensor
    float pressure = readPressureSensor();
    Serial.print("pressure :");
    Serial.println(pressure);
    if (pressure < SUCTION_THRESHOLD) {
      activateAlarm();
    }
    if (!digitalRead(tamper)) {
      activateAlarm();
    }
  }
}

void activateSystem() {
  Serial.println("System activated");
  digitalWrite(RFID_LED_PIN, HIGH); // Green LED
  lockServo.write(180); // Engage lock
  digitalWrite(SUCTION_PIN, HIGH); // Start suction motor
  systemActivated = true;
}

void deactivateSystem() {
  Serial.println("System deactivated");
  digitalWrite(RFID_LED_PIN, LOW); // Turn off LED
  lockServo.write(0); // Disengage lock
  digitalWrite(SUCTION_PIN, LOW); // Stop suction motor
  systemActivated = false;
}

float readPressureSensor() {
  // Simulated function to read pressure sensor
  return analogRead(PRESSURE_SENSOR_PIN) * 0.0048828125;
}

void activateAlarm() {
  Serial.println("Tampering detected");
  digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
  delay(600); // Alarm for 10 minutes
  digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
}
