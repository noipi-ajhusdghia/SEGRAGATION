#include <Servo.h>

// Servo
Servo sorterServo;
int servoPin = 11;

// PNP sensor input (HIGH when object detected)
int sensorPin = 6;

// Servo angles
int LEFT_POS  = 0;     // plastic
int RIGHT_POS = 180;   // paper

// Smooth movement speed
int stepDelay = 8;     // lower = faster, higher = smoother

// Debounce smoothing
int stableReadings = 0;
int requiredStable = 5; // must read same state 5 times

bool lastState = LOW;

void setup() {
  sorterServo.attach(servoPin);
  sorterServo.write(90);  // neutral
  pinMode(sensorPin, INPUT);
}

void smoothMove(int targetAngle) {
  int current = sorterServo.read();

  if (current < targetAngle) {
    for (int pos = current; pos <= targetAngle; pos++) {
      sorterServo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = current; pos >= targetAngle; pos--) {
      sorterServo.write(pos);
      delay(stepDelay);
    }
  }
}

void loop() {

  // Read sensor
  int reading = digitalRead(sensorPin);

  // Smooth the sensor: only accept change after several consistent readings
  if (reading == lastState) {
    stableReadings++;
  } else {
    stableReadings = 0;
  }

  // Once stable, act
  if (stableReadings >= requiredStable) {

    if (reading == HIGH) {
      // PLASTIC
      smoothMove(LEFT_POS);
    } else {
      // PAPER
      smoothMove(RIGHT_POS);
    }

    stableReadings = 0;
    lastState = reading;
  }

  delay(10);
}
