#include <AccelStepper.h>

// --- Pin Connections ---
const int stepPin = 9;    // PUL+
const int dirPin = 8;     // DIR+
const int enPin = 7;      // EN+

// --- Calibration ---
// Calculation: 400 steps (1/2 step) / 0.2 cm (lead screw pitch) = 2000 steps per cm.
const float stepsPerCm = 500.0; 

// Define the stepper (1 = Driver interface)
AccelStepper stepper(1, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW); // Engage motor to hold the 15kg load

  // --- 11.2V MAX SPEED TUNING ---
  // MaxSpeed: 2500 is likely the limit for 11.2V under load. 
  // If it stalls/buzzes, lower this to 2000.
  stepper.setMaxSpeed(1000);     
  
  // Acceleration: A lower value (500) gives the motor time to 
  // gain momentum with the 15kg weight before hitting top speed.
  stepper.setAcceleration(1000);  
  
  Serial.println("--- 11.2V Max Speed Controller Ready ---");
  Serial.println("Enter CM distance (Positive = Up, Negative = Down):");
}

void loop() {
  if (Serial.available() > 0) {
    float moveDistance = Serial.parseFloat();
    
    if (moveDistance != 0) {
      long targetSteps = moveDistance * stepsPerCm;
      Serial.print("Targeting: ");
      Serial.print(moveDistance);
      Serial.println(" cm at Max Speed.");
      
      stepper.move(targetSteps);
    }
  }

  // Mandatory: keeps the pulses flowing to the driver
  stepper.run();
}