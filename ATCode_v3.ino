#include <Servo.h>
//#include <Wire.h>
//#include <Adafruit_PWMServoDriver.h>
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Servo spinArm;
int angleArm = 0;
int angleWheel = 0;
Servo spinWheel;
Servo clampLeft;
Servo clampRight;
Servo flipper;
#define ButtonForward 8
#define ButtonBackward 7
#define LED 6
int pos = 0; //Variable used to store the position of the motor
int lastButtonStateForward;    // the previous state of button
int currentButtonStateForward; // the current state of button
int lastButtonStateBackward;    // the previous state of button
int currentButtonStateBackward; // the current state of button
int lastDirection; //The last direction that the arm was in, if it's at 1 was at right of book

void setup()   {
  pinMode(6, OUTPUT);
  pinMode(8, INPUT);
  spinWheel.attach(2);
  spinArm.attach(3);
  clampLeft.attach(4);
  clampRight.attach(5);
  flipper.attach(6);
  //pwm.begin();
  Serial.begin(9600);
  lastDirection = 1; //Set by default to be on right side of book
  spinArm.write(90);
}

void loop() {
  //Note: for the arm angle 0 is all the way to the right, 90 is upright, and 180 is all the way to the left, like on a cartesian plane
  lastButtonStateForward    = currentButtonStateForward;      // save the last state
  lastButtonStateBackward    = currentButtonStateBackward;      // save the last state
  currentButtonStateForward = digitalRead(ButtonForward); // read new state
  currentButtonStateBackward = digitalRead(ButtonBackward); // read new state
  if (lastButtonStateForward == HIGH && currentButtonStateForward == LOW) {
    digitalWrite(LED, HIGH);
    Serial.println("The forxward button is pressed");
    //if (lastDirection == 0) {
    spinArm.write(0);              // tell servo to go to right position
    delay(500);                   // waits 500 ms for the servo to reach the position
    //}
    lastDirection = 1;
    delay(50);
    spinWheel.write(50);
    delay(3000);
    spinWheel.write(90);
    //This code is meant to reduce shaking (idek if it'll work LOL)
    {
      spinArm.write(90);
      spinArm.write(89);
      spinArm.write(91);
      spinArm.write(90);
    }
  }
  else if (lastButtonStateBackward == HIGH && currentButtonStateBackward == LOW) { //Seems to do this after the button has been released
    digitalWrite(LED, HIGH);
    Serial.println("The backward button is pressed");
    //if (lastDirection == 1) {
    spinArm.write(180);
    delay(500);                       // waits 500 ms for the servo to reach the position
    //}
    lastDirection = 0;
    delay(2000);
    spinWheel.write(130);
    delay(3000);
    spinWheel.write(90);
    {
      spinArm.write(90);
      spinArm.write(89);
      spinArm.write(91);
      spinArm.write(90);
    }
  }
  else {
    digitalWrite(6, LOW);
  }
  delay (10);
}
