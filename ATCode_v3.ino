#include <Servo.h>
//#include <Wire.h>
//#include <Adafruit_PWMServoDriver.h>
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Servo spinArm;
Servo spinWheel;
Servo clampLeft;
Servo clampRight;
Servo flipper;
#define ButtonForward 7
#define ButtonBackward 8
#define LED 9
/*#define SERVOMIN  75 // This is the 'minimum' pulse length count (out of 4096)
  #define SERVOMAX  435 // This is the 'maximum' pulse length count (out of 4096) //BOTH ABOVE FOR ARM
  #define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
  #define USMIN  2000 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
  #define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
  int angleArm = 0;
  int angleWheel = 0;*/
int pos = 0; //Variable used to store the position of the motor
int lastButtonStateForward;    // the previous state of button
int currentButtonStateForward; // the current state of button
int lastButtonStateBackward;    // the previous state of button
int currentButtonStateBackward; // the current state of button
int lastDirection; //The last direction that the arm was in, if it's at 1 was at right of book

void setup()   {
  pinMode(9, OUTPUT); //Output for the LED
  pinMode(8, INPUT); //Button input
  pinMode(7, INPUT); //Button input
  spinWheel.attach(2);//The wheel servo is assigned pin 2
  spinArm.attach(3); //The arm servo is assigned pin 3
  clampLeft.attach(4); //The clamp for the left side of the book is assigned pin 4
  clampRight.attach(5); //The clamp for the right side of the book is assigned pin 5
  flipper.attach(10);  //The flipper is assigned pin 10
  //pwm.begin();
  Serial.begin(9600);
  lastDirection = 1; //Set by default to be on right side of book
  spinArm.write(90);
  flipper.write(0);
  digitalWrite(LED, LOW);
  clampLeft.write(0);
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
    clampRight.write(0); //Releases the clamp from the right side of the book (hopefully)
    delay(50);
    { //Runs wheel and flipper at same time for 1 second
      spinWheel.write(80); //Starts to create the ridge under the page
    }
    delay(1000);        //Wait 1000ms or 1s
    spinWheel.write(90);
    clampRight.write(90);  //Lowers right clamp
    clampLeft.write(90); //Raises the left clamp
    delay(1000);
    spinArm.write(90);
    delay(1000);
    flipper.write(180); //Moves it all the way to the right when viewing from right side up
    delay(1000);
    clampLeft.write(0); //Lowers left clamp
    delay(1000);
    flipper.write(0); //Flips the page
 
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
    flipper.write(180);
    delay(1000);
    clampLeft.write(90);
    delay(50);
    { //Runs wheel and flipper at same time for 1 second
      spinWheel.write(100); //Starts to create the ride under the page
    }
    delay(1000);
    spinWheel.write(90);
    clampRight.write(0);
    clampLeft.write(0); //DIFFERENT BECAUSE THIS CLAMPS BACK DOWN BEFORE THE PAGE IS FLIPPED
    

    delay(1000);
    flipper.write(0);
    delay(1000);
    clampRight.write(90);
    spinArm.write(90);

  }
  else {
    digitalWrite(9, LOW);
  }
  delay (10);
}
