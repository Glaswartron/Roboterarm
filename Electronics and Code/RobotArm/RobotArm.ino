#include <AccelStepper.h>
#include <Servo.h>

#define STEPPER_PIN0 1
#define STEPPER_PIN1 5
#define STEPPER_PIN2 6

#define JOYSTICK_PIN_X A0
#define JOYSTICK_PIN_Y A1

#define ARM_1_SERVO_PIN 8
#define ARM_2_SERVO_PIN 7

#define ARM_SWITCH_BUTTON_PIN 4

AccelStepper stepper(STEPPER_PIN0, STEPPER_PIN1, STEPPER_PIN2);
Servo arm1Servo;
Servo arm2Servo;

float servo1Pos;
float servo2Pos;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // put your setup code here, to run once:
  arm1Servo.attach(ARM_1_SERVO_PIN);
  arm1Servo.write(180);
  servo1Pos = 180.0;

  arm2Servo.attach(ARM_2_SERVO_PIN);
  arm2Servo.write(120);
  servo2Pos = 120.0;

  stepper.setMaxSpeed(50);

  pinMode(ARM_SWITCH_BUTTON_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  //stepper.setSpeed(50);
  //stepper.runSpeed();

  int x = analogRead(JOYSTICK_PIN_X);
  int y = analogRead(JOYSTICK_PIN_Y);

  int xCentered = x - 502;
  int yCentered = y - 513; 

  if (abs(xCentered) >= 100) {
    int xMapped = map(xCentered, -502, 521, -50, 50);

    //Serial.print("x_mapped: "); Serial.print(x_mapped);
    //Serial.print("\n");
    //Serial.print("y_mapped: "); Serial.print(y_mapped);
    //Serial.print("\n");

    stepper.setSpeed(xMapped);
    stepper.runSpeed();
  } else {
    stepper.setSpeed(0);
  }

  if (abs(yCentered) >= 100) {

      float yMapped = mapFloat(yCentered, -513.0, 523.0, -0.05, 0.05);

      int armSwitchButtonState = digitalRead(ARM_SWITCH_BUTTON_PIN);

      if (armSwitchButtonState == LOW) {
        if (servo1Pos + -1 * yMapped <= 180.0 && servo1Pos + -1 * yMapped >= 0.0) {
          servo1Pos += -1 * yMapped;
        }
        arm1Servo.write(servo1Pos);
      } else {
        if (servo2Pos + -1 * yMapped <= 180.0 && servo2Pos + -1 * yMapped >= 0.0) {
          servo2Pos += -1 * yMapped;
        }
        arm2Servo.write(servo2Pos);
      }
  }



  //Serial.print("X: "); Serial.print(x); Serial.print(",  "); Serial.print("Y: "); Serial.print(y);
  //Serial.print("\n");
}