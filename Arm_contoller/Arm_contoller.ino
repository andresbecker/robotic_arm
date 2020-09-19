/*
 Read Joystick Status
 Reference: https://www.web-robotica.com/arduino/como-utilizar-el-modulo-joystick-con-arduino

 Joystick Values 0 to 1023
 Transform this values to degrees in [0,180]
 */

#include <Servo.h>

//Declare Servo pin in Arduiono (Out signal)
int xServoPin = 3;
int yServoPin = 5;
int xServoPin2 = 6;
int yServoPin2 = 9;
int pincerServoPin = 10;

//Create Servo object
Servo xServo;
Servo yServo;
Servo xServo2;
Servo yServo2;
Servo pincerServo;

float xRate = 0.0;
float xSum = 0.0;
float yRate = 0.0;
float ySum = 0.0;

float xRate2 = 0.0;
float xSum2 = 0.0;
float yRate2 = 0.0;
float ySum2 = 0.0;

// Set input for Joystick
int xPin = A0;
int yPin = A1;
//int buttonPin = 2;
int xPin2 = A2;
int yPin2 = A3;

// Set input pin for Variable resistance (Pincer)
int pincerPin = A4;
int pincerPos = 0;
//int pincerAng = AngLimSup;
int pincerAng = 90;

// Set input pin for Variable resistance (arm movement speed)
int speedPin = A5;
float MovSpeed = 1;
float JoystickRate = 1;
float temp = 1;

// Set status vars
int xPos = 0;
int yPos = 0;
//int buttonStat = 0;
int xPos2 = 0;
int yPos2 = 0;

// Status in degrees
int xAng = 130;
int yAng = 80;
int xAng2 = 25;
int yAng2 = 155;


void setup() {
  //init 9600 bps serial comunication  
  Serial.begin(9600);

  //set Arduino Pins mode as INPUT
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  //pinMode(buttonPin, INPUT_PULLUP);
  pinMode(xPin2, INPUT);
  pinMode(yPin2, INPUT);

  //Pincer
  pinMode(pincerPin, INPUT);
  //Speed of the arm
  pinMode(speedPin, INPUT);

  //Servo setups
  xServo.attach(xServoPin);
  yServo.attach(yServoPin);
  xServo2.attach(xServoPin2);
  yServo2.attach(yServoPin2);
  pincerServo.attach(pincerServoPin);
  //Init servo position in 90 degrees
  xServo2.write(xAng2);
  yServo2.write(yAng2);
  xServo.write(xAng);
  yServo.write(yAng);

  pincerServo.write(pincerAng);
  
}

void loop() {

  // Read Joysticks input
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);
  xPos2 = analogRead(xPin2);
  yPos2 = analogRead(yPin2);
  //buttonStat = digitalRead(buttonPin);
  pincerPos = analogRead(pincerPin);
  MovSpeed = analogRead(speedPin);


  // Speed of the arm
  //JoystickRate = round(MovSpeed / 10.23) / 20.0;
  JoystickRate = round(5 * MovSpeed / 1023.0);
  /*Serial.print("ArmSpeed: ");
  Serial.print(JoystickRate);
  Serial.println("");*/


  // Process for firs Joystick and servos 1 & 2
  xRate = ((float)xPos - 510.0) / 510.0;
  yRate = ((float)yPos - 510.0) / 510.0;

  // Process for second Joystick and servos 3 & 4
  xRate2 = ((float)xPos2 - 510.0) / 510.0;
  yRate2 = ((float)yPos2 - 510.0) / 510.0;

  // For lower forearm forward and backward movement
  xSum = xRate * JoystickRate;
  if ((xSum < 0.9) && (-0.9 < xSum)){
    xSum = 0;
  }
  xAng = xAng - round(xSum);
  if (xAng < 10) {
    xAng = 10;
  }
  else if (xAng > 135) {
    xAng = 135;
  }
  /* For calibration
  Serial.print("xAng: ");
  Serial.print(xAng);
  Serial.println("");
  */

  // For rotate the whole arm
  ySum = yRate2 * JoystickRate;
  if ((ySum < 0.9) && (-0.9 < ySum)){
    ySum = 0;
  }
  yAng = yAng - round(ySum);
  if (yAng < 10) {
    yAng = 10;
  }
  else if (yAng > 170) {
    yAng = 170;
  }
  /* For calibration
  Serial.print("yAng: ");
  Serial.print(yAng);
  Serial.println(""); */
  
  
  // For upper forearm forward and backward movement
  xSum2 = xRate2 * JoystickRate;
  if ((xSum2 < 0.9) && (-0.9 < xSum2)){
    xSum2 = 0;
  }
  xAng2 = xAng2 + round(xSum2);
  if (xAng2 < 5) {
    xAng2 = 5;
  }
  else if (xAng2 > 140) {
    xAng2 = 140;
  }

  // For rotation of the upper forearm
  ySum2 = yRate * JoystickRate;
  if ((ySum2 < 0.9) && (-0.9 < ySum2)){
    ySum2 = 0;
  }
  yAng2 = yAng2 + round(ySum2);
  if (yAng2 < 10) {
    yAng2 = 10;
  }
  else if (yAng2 > 160) {
    yAng2 = 160;
  }

  //Pincer
  pincerAng = round(170.0 * ((float)pincerPos / 1023.0));
  if (pincerAng < 40) {
    pincerAng = 40;
  }
  else if (pincerAng > 120) {
    pincerAng = 120;
  }
  /*
  Serial.print("Pincer: ");
  Serial.print(pincerAng);
  Serial.println("");
  Serial.println("");
  */

  xServo.write(xAng);
  yServo.write(yAng);
  xServo2.write(xAng2);
  yServo2.write(yAng2);
  pincerServo.write(pincerAng);

  delay(15);
  //delay(200);
}
