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
int pincerServoPin = 6;
//Create Servo object
Servo xServo;
Servo yServo;
Servo pincerServo;

float JoystickRate = 10.0;
float xRate = 0;
float xSum = 0;
float yRate = 0;
float ySum = 0;

int AngLimInf = 0;
int AngLimSup = 170;

// Set input for Joystick
int xPin = A0;
int yPin = A1;
int buttonPin = 2;

// Set input pin for Variable resistance (Pincer)
int pincerPin = A2;
int pincerPos = 0;
int pincerAng = AngLimSup;

// Set status vars
int xPos = 0;
int yPos = 0;
int buttonStat = 0;

// Status in degrees
int xAng = 90;
int yAng = 90;


void setup() {
  //init 9600 bps serial comunication  
  Serial.begin(9600);

  //set Arduino Pins mode as INPUT
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  //Pincer
  pinMode(pincerPin, INPUT);

  //Servo setups
  xServo.attach(xServoPin);
  yServo.attach(yServoPin);
  pincerServo.attach(pincerServoPin);
  //Init servo position in 90 degrees
  xServo.write(xAng);
  yServo.write(yAng);
  pincerServo.write(pincerAng);
  
}

void loop() {
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);
  buttonStat = digitalRead(buttonPin);
  pincerPos = analogRead(pincerPin);

  xRate = ((float)xPos - 510.0) / 510.0;
  yRate = ((float)yPos - 510.0) / 510.0;

  xSum = xRate * JoystickRate;
  if ((xSum < 1) && (-1 < xSum)){
    xSum = 0;
  }
  ySum = yRate * JoystickRate;
  if ((ySum < 1) && (-1 < ySum)){
    ySum = 0;
  }

  xAng = xAng + round(xSum);
  if (xAng < AngLimInf) {
    xAng = AngLimInf;
  }
  else if (xAng > AngLimSup) {
    xAng = AngLimSup;
  }

  yAng = yAng + round(ySum);
  if (yAng < AngLimInf) {
    yAng = AngLimInf;
  }
  else if (yAng > AngLimSup) {
    yAng = AngLimSup;
  }

  //Pincer
  pincerAng = round((float)AngLimSup * ((float)pincerPos / 1023.0));
  

  xServo.write(xAng);
  yServo.write(yAng);
  pincerServo.write(pincerAng);

  /*
  Serial.print("X: ");
  Serial.print(xPos);
  //Serial.print(" | Y: ");
  //Serial.print(yPos);
  //Serial.print(" | Button: ");
  //Serial.print(buttonStat);
  Serial.println("");
  Serial.print("xAng: ");
  Serial.print(xAng);
  Serial.println("");

  Serial.print("xSum: ");
  Serial.print(xSum);
  Serial.println("");
  
  Serial.print("xRate: ");
  Serial.print(xRate);
  Serial.println("");
  
  Serial.println("");
  */
  Serial.print("PincerPos: ");
  Serial.println(pincerPos);
  Serial.print("PincerAng: ");
  Serial.println(pincerAng);
  Serial.println("");

  delay(50);
  //delay(500);
}
