#include <Servo.h>
Servo spinArm;
Servo spinWheel;
Servo clampLeft;
Servo clampRight;
Servo flipper;
#define ButtonForward 7
#define ButtonBackward 8
#define LED 9
int pos = 0; //Variable used to store the position of the motor
int lastButtonStateForward;    // the previous state of button
int currentButtonStateForward; // the current state of button
int lastButtonStateBackward;    // the previous state of button
int currentButtonStateBackward; // the current state of button
int lastDirection; //The last direction that the arm was in, if it's at 1 was at right of book

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int MinFreq;
int MaxFreq;
int Delta;
int wheelSpinTime;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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



  //MODIFIABLE VALUES:
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MinFreq = 600;        //How far the wheel goes down on the left side of the book, lower values means it goes lower
  MaxFreq = 3900;       //How far the wheel goes down on the right side of the book, higher values means it goes lower
  Delta = 15;           //How far off of 90 the wheel write value is, the higher the delta the faste the wheel spinds
  wheelSpinTime = 750;  //How long the wheel spins in milliseconds
  //IDEAL: 625, 3850, 15, 750
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




}

void loop() {
  //Note: for the arm angle 0 is all the way to the right, 90 is upright, and 180 is all the way to the left, like on a cartesian plane or a unit circle
  lastButtonStateForward    = currentButtonStateForward;      // save the last state
  lastButtonStateBackward    = currentButtonStateBackward;      // save the last state
  currentButtonStateForward = digitalRead(ButtonForward); // read new state
  currentButtonStateBackward = digitalRead(ButtonBackward); // read new state
  if (lastButtonStateForward == HIGH && currentButtonStateForward == LOW) {
    digitalWrite(LED, HIGH);
    Serial.println("The forward button is pressed");
    spinArm.writeMicroseconds(MinFreq);     // tell servo to go to left position
    delay(500);                   // waits 500 ms for the servo to reach the position
    clampRight.write(0); //Releases the clamp from the right side of the book (hopefully)
    delay(500);
    { //Runs wheel and flipper at same time for 1 second
      spinWheel.write(90 - Delta); //Starts to create the ridge under the page
    }
    delay(wheelSpinTime);        //Wait 1000ms or 1s
    spinWheel.write(90);
    delay(500);
    clampLeft.write(90); //Raises the left clamp
    delay(1000);
    spinArm.write(90);
    delay(1000);
    clampRight.write(90);  //Lowers right clamp
    flipper.write(180); //Moves it all the way to the right when viewing from right side up
    delay(1000);
    clampLeft.write(0); //Lowers left clamp
    delay(1000);
    flipper.write(0); //Flips the page


  }
  else if (lastButtonStateBackward == HIGH && currentButtonStateBackward == LOW) { //Seems to do this after the button has been released
    digitalWrite(LED, HIGH);
    Serial.println("The backward button is pressed");
    spinArm.writeMicroseconds(MaxFreq);
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
      spinWheel.write(90 + Delta + 5); //Starts to create the ride under the page
    }
    delay(wheelSpinTime);
    spinWheel.write(90);
    delay(500);
    spinArm.write(90);
    clampRight.write(0);
    delay(500);
    clampLeft.write(0);
    delay(1000);
    flipper.write(0);
    delay(1000);
    clampRight.write(90);
  }
  else {
    digitalWrite(9, LOW);
  }
  delay (10);
}
