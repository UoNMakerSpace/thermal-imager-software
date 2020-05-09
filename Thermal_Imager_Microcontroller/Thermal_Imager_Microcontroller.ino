/* Arduino Uno + CNC Shield V3 */
//Uses the AccelStepper library
//http://www.airspayce.com/mikem/arduino/AccelStepper/

#include "AccelStepper.h" 
// AccelStepper Setup
AccelStepper stepperX(1, 3, 6);   // 1 = STEP & DIR, Pin 2 connected to STEP pin of DriverY, Pin 5 connected to DIR pin of DriverY

//Pins
const int ENABLE_PIN = 8; // common enable pin for all motors
const int step_pinX  = 3; // Pin 3 connected to Y Steps pin on CNC Shield
const int dir_pinX = 6;   // Pin 5 connected to Y Direction pin X
const int SENSOR = 11 ;  // Hall Sensor on Pin 11 (Z+ limit on shield)

// Stepper position variables
long PositionX;  // Used to store the X step value entered in the Serial Monitor
int HomeSensor ; // define numeric variable for Hall sensor
int move_finished = 1; // Used to check if move is completed

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(dir_pinX, OUTPUT);
  pinMode(step_pinX, OUTPUT);
  pinMode (SENSOR, INPUT_PULLUP) ; //define SENSORY as input and set pullup resistor
  Serial.begin(57600);  // Start the Serial monitor with speed of 57600 Bauds

  Serial.println("Ready");

  //  Set Max Speed and Acceleration of each Stepper
  stepperX.setMaxSpeed(1600.0);      // Set Max Speed of X axis
  stepperX.setAcceleration(3200.0);  // Acceleration of X axis
  digitalWrite (ENABLE_PIN, LOW) ; //for the CNC Shield, LOW is enabled
  digitalWrite (SENSOR, HIGH) ;
}
void loop() {

  HomeSensor = digitalRead(SENSOR) ; // read limit sensor
  while (Serial.available() > 0)  { // Check if values are available in the Serial Buffer
    move_finished = 0; // Set variable for checking move of the Steppers
    PositionX = Serial.parseInt(); // Put First numeric value from buffer in PositionX variable
    stepperX.moveTo(PositionX);  // Set new move position for X Stepper
    delay(100);  // Pause before moving the Steppers
  }

  if ((stepperX.distanceToGo() != 0) && (HomeSensor == HIGH))  {
    digitalWrite (ENABLE_PIN, LOW) ; //for the CNC Shield, LOW is enabled
    stepperX.run();  // Move Stepper X into position

  }
  else if (HomeSensor == LOW )  {
    stepperX.stop();

    if (!digitalRead(HomeSensor)); {
      digitalWrite (ENABLE_PIN, LOW) ; //for the CNC Shield, LOW is enabled
      stepperX.move(4);
      stepperX.run();
    }
    stepperX.setCurrentPosition(0);  //reset home position
    digitalWrite (ENABLE_PIN, HIGH) ; //for the CNC Shield, LOW is enabled
  }

  // If move is completed display message on Serial Monitor
  if ((move_finished == 0) && (stepperX.distanceToGo() == 0) ) {
    digitalWrite (ENABLE_PIN, HIGH) ; //for the CNC Shield, LOW is enabled
    Serial.println("Completed");  // Get ready for new Serial monitor values
    move_finished = 1; // Reset move variable
  }
}
