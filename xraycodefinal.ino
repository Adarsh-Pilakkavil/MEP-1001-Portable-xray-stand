#include <AccelStepper.h>

// Pins
const int stepPin = 9; 
const int dirPin = 8;  
const int enPin = 7;   

// Calibration: (400 steps/rev) / (0.2 cm lead screw pitch) = 2000
const float stepsPerCm = 500.0; 

AccelStepper stepper(1, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW); // Lock motor to hold the 15kg weight

  // Tuned for 14.8V Battery
  stepper.setMaxSpeed(10000);      // High top speed
  stepper.setAcceleration(750);  // Smooth ramp-up
  
  Serial.println("--- X-RAY STAND SYSTEM ONLINE ---");
  Serial.println("Commands: Enter CM (Positive for UP, Negative for DOWN)");
}

void loop() {
  if (Serial.available() > 0) {
    float moveCm = Serial.parseFloat();
    
    if (moveCm != 0) {
      long targetSteps = moveCm * stepsPerCm;
      Serial.print("Moving: ");
      Serial.print(moveCm);
      Serial.println(" cm...");
      
      // Move relative to current position
      stepper.move(targetSteps);
    }
  }

  // Must be called as fast as possible to keep motor turning
  stepper.run();
}
