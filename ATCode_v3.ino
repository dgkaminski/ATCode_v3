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
#define ButtonForward 7
#define ButtonBackward 8
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
  spinWheel.attach(2);//The wheel servo is assigned pin 2
  spinArm.attach(3); //The arm servo is assigned pin 3
  clampLeft.attach(4); //The clamp for the left side of the book is assigned pin 4
  clampRight.attach(5); //The clamp for the right side of the book is assigned pin 5
  flipper.attach(6);  //The flipper is assigned pin 6
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
    Serial.println("The forward button is pressed");
    spinArm.write(0);              // tell servo to go to right position
    delay(500);                   // waits 500 ms for the servo to reach the position
    if (lastDirection == 0) {
      lastDirection = 1;    //Sets to see the system as being to the right
      flipper.write(180); //Moves it all the way to the right when viewing form upside down
    }
    clampRight.write(0); //Releases the clamp from the right side of the book (hopefully)
    delay(50);
    { //Runs wheel and flipper at same time for 1 second
      spinWheel.write(85); //Starts to create the ride under the page
    }
    delay(1000);        //Wait 1000ms or 1s
    spinWheel.write(90);
    clampLeft.write(180); //Raises the left clamp
    flipper.write(0); //Flips the page
    clampRight.write(90);  //Lowers right clamp
    clampLeft.write(90); //Lowers left clamp
    spinArm.write(90);
  }
  else if (lastButtonStateBackward == HIGH && currentButtonStateBackward == LOW) { //Seems to do this after the button has been released
    digitalWrite(LED, HIGH);
    Serial.println("The backward button is pressed");
    spinArm.write(180);
    delay(500);                       // waits 500 ms for the servo to reach the position
    if (lastDirection == 1) {
      lastDirection = 0;
      flipper.write(0);
    }
    delay(1000);
    clampLeft.write(180);
    delay(50);
    { //Runs wheel and flipper at same time for 1 second
      spinWheel.write(105); //Starts to create the ride under the page
    }
    delay(1000);
    spinWheel.write(90);
    clampLeft.write(90); //DIFFERENT BECAUSE THIS CLAMPS BACK DOWN BEFORE THE PAGE IS FLIPPED
    flipper.write(180);
    clampRight.write(90);
    spinArm.write(90);
  }
  else {
    digitalWrite(6, LOW);
  }
  delay (10);
}
