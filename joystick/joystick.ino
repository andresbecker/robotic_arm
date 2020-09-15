/*
 Read Joystick Status
 Reference: www.web-robotica.com

 Joystick Values 0 to 1023
 Transform this values to degrees in [0,180]
 */

float JoystickRate = 10.0;
float xRate = 0;
float xSum = 0;

int AngLimInf = 0;
int AngLimSup = 180;

// Set input
int xPin = A0;
int yPin = A1;
int buttonPin = 2;

// Set status vars
int xPos = 0;
int yPos = 0;
int buttonStat = 0;

// Status in degrees
int xAng = 90;
int yAng = 0;

void setup() {
  //init 9600 bps serial comunication  
  Serial.begin(9600);

  //set Arduino Pins mode as INPUT
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);
  buttonStat = digitalRead(buttonPin);

  xRate = ((float)xPos - 510.0) / 510.0;

  xSum = xRate * JoystickRate;
  if ((xSum < 1) && (-1 < xSum)){
    xSum = 0;
  }
  
  xAng = xAng + round(xSum);
  if (xAng < AngLimInf) {
    xAng = AngLimInf;
  }
  else if (xAng > AngLimSup) {
    xAng = AngLimSup;
  }

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

  delay(500);
}
