/*
 Program to control a 4 axis + pincer robotic arm with 2 Joysticks and potentiometer.
 19.09.2020
 Made by Andres Becker

 Joystick and Potentiometer Values: 0 to 1023
 Servo input: degrees in [0,180]
 
 Read Joystick Status
 Reference: https://www.web-robotica.com/arduino/como-utilizar-el-modulo-joystick-con-arduino
 */

#include <Servo.h>

//Arduino Pins for Servo signal (Out)
int xServoPin = 3;
int yServoPin = 5;
int xServoPin2 = 6;
int yServoPin2 = 9;
int pincerServoPin = 10;

// Arduino Pins for Joysticks and Potentiometers (input)
int xPin = A0; //Joystick 1, x
int yPin = A1; //Joystick 1, y
int xPin2 = A2; //Joystick 2, x
int yPin2 = A3; //Joystick 2, y
int pincerPin = A4; //Potentiometer 1 for pincer
int speedPin = A5; //Potentiometer 2 for arm speed

//Create Servo object
Servo xServo; //Arm rotation
Servo yServo; //Lower forearm forward and backward
Servo xServo2; //Wrist rotation
Servo yServo2; //Upper forearm upp and down
Servo pincerServo; //Pincer

// Vars for controlling the servos movement with the Joystics
// First Joystick and first two servos
float xRate = 0.0;
float xSum = 0.0;
float yRate = 0.0;
float ySum = 0.0;
// Second Joystick and 3 &4 servos
float xRate2 = 0.0;
float xSum2 = 0.0;
float yRate2 = 0.0;
float ySum2 = 0.0;

// Pincer vars
int pincerPos = 0;
int pincerAng = 90;

// Arm speed vars
float MovSpeed = 1;
float JoystickRate = 1;
float temp = 1;

// Arm posicion vars
int xPos = 0;
int yPos = 0;
int xPos2 = 0;
int yPos2 = 0;

// Initial arm posicion in degrees
int xAng = 130;
int yAng = 80;
int xAng2 = 25;
int yAng2 = 155;




void setup() {
  //init serial comunication  
  Serial.begin(9600);

  //set Arduino Pins mode as INPUT for Joysticks
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(xPin2, INPUT);
  pinMode(yPin2, INPUT);
  //set Arduino Pins mode as INPUT for potentiometers
  pinMode(pincerPin, INPUT); //Pincer
  pinMode(speedPin, INPUT); //Arm Speed

  //Assign Arduiono pins to each Servo
  xServo.attach(xServoPin);
  yServo.attach(yServoPin);
  xServo2.attach(xServoPin2);
  yServo2.attach(yServoPin2);
  pincerServo.attach(pincerServoPin);
  
  //Move servos to initial posicion
  xServo2.write(xAng2);
  yServo2.write(yAng2);
  xServo.write(xAng);
  yServo.write(yAng);
  pincerServo.write(pincerAng);
}

void loop() {

  // Read Joysticks and potentiometers status
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);
  xPos2 = analogRead(xPin2);
  yPos2 = analogRead(yPin2);
  pincerPos = analogRead(pincerPin);
  MovSpeed = analogRead(speedPin);

  // Speed of the arm
  // JoystickRate is var that scale the Joystick movement rate
  JoystickRate = round(5 * MovSpeed / 1023.0);
  /*Serial.print("ArmSpeed: ");
  Serial.print(JoystickRate);
  Serial.println("");*/

  //Joysticks rates
  // Joystick 1 servo movement rate for each component
  xRate = ((float)xPos - 510.0) / 510.0;
  yRate = ((float)yPos - 510.0) / 510.0;
  // Joystick 1 servo movement rate for each component
  xRate2 = ((float)xPos2 - 510.0) / 510.0;
  yRate2 = ((float)yPos2 - 510.0) / 510.0;

  /*
   * The next part set the angle for each servo according to the
   * Joysticks and potentiometer.
   */
   
  // Servo 2, For lower forearm forward and backward movement
  xSum = xRate * JoystickRate;
  // If the summed degrees are close to 0 then,
  // we set it to 0 to avoid vibratory movements.
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

  // Servo 1, For rotate the whole arm
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
  
  
  // Servo 3, For upper forearm forward and backward movement
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
  /* For calibration
  Serial.print("xAng2: ");
  Serial.print(xAng2);
  Serial.println(""); */

  // Servo 4, For rotation of the upper forearm
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
  /* For calibration
  Serial.print("yAng2: ");
  Serial.print(yAng2);
  Serial.println(""); */

  //Servo 5, Pincer
  pincerAng = round(170.0 * ((float)pincerPos / 1023.0));
  if (pincerAng < 40) {
    pincerAng = 40;
  }
  else if (pincerAng > 120) {
    pincerAng = 120;
  }
  /* For calibration
  Serial.print("Pincer: ");
  Serial.print(pincerAng);
  Serial.println("");*/

  // Move the servos the indicated angles
  xServo.write(xAng);
  yServo.write(yAng);
  xServo2.write(xAng2);
  yServo2.write(yAng2);
  pincerServo.write(pincerAng);

  //Give servos time to move
  delay(15);
  //delay(200);
}
