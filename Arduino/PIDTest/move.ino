// pre: the diameter of the wheel is motorDiameter
//      the radius to turn is DiameterToTurn
//      the angle the motor should turn could be calculated as 
//      motorDiameter * angleMotor =  angleTurning * DiameterToTurn
// 63 * 3.1416 millimeter per turn
// 63 * 3.1416 / 360 millimeter per degree
// 360 / 63 * 3.1416 degree per millimeter 
// const int motorDiameter = 63; // in millimeter
// const int Pi = 314;

const float mapConstant = 1.8189;

void moveBegin() {
  motorLeft.begin();
  motorRight.begin();

  //turn the PID on
  Input = 0;
  Setpoint = 0;
  myPID.SetMode(AUTOMATIC);

}    

void movePID(int errorInput) {
  // Input need to set to receive info
  Input = errorInput;
  myPID.Compute();
  Serial.println(Output);
  motorRight.RunSpeed(carSpeed + Output);
}

// pre: give the distance between two points
void moveForward(int millimeter) {
  int degreeToTurn = millimeter * mapConstant;
  degreeCounterL -= degreeToTurn;
  degreeCounterR += degreeToTurn;
  motorLeft.MoveTo( degreeCounterL, carSpeed);
  motorRight.MoveTo(degreeCounterR, carSpeed);
  serialFeedback();
  delay(processSpeed);
}

void moveBackward(int millimeter) {
  moveForward(-millimeter);
}

// post: turn to left for 90 degree;

const int turnAroundDegree = 2352;
const double oneCircle = 360.0;
void turnCountClockwise() {
  turnToRight(-90);
}

// post: turn to right for 90 degree;
void turnClockwise() {
  turnToRight(90);
}

// post: safely turn 180 degree without touching the object
void turnAround() {
  moveBackward(20);
  turnToRight(360);
}

void turnCW45() {
  turnToRight(45);
} 

void turnCCW45() {
  turnToRight(-45);
}

void turnCW135() {
  turnToRight(135);
}

void turnCCW135() {
  turnToRight(-135);
}

void turnToLeft(int angleTurning) {
  turnToRight(-angleTurning);
}

// post: Turning Right for the given angle
//       It will turn left if input is negative
void turnToRight(int angleTurning) {
  int degreeTurning = (int) (angleTurning / oneCircle * turnAroundDegree);
  degreeCounterL += degreeTurning;
  degreeCounterR += degreeTurning;
  makeTurn( degreeCounterL, degreeCounterR);
}

// pre: private helper for turning
void makeTurn(int degreeL, int degreeR) {
  motorLeft.MoveTo( degreeL, carSpeed);
  motorRight.MoveTo(degreeR, carSpeed);
  serialFeedback();
  delay(processSpeed);
} 


void startSlowly() {
  for (int i = 0; i <= carSpeed / 10 ; i++) {
    int current = i * 10;
    motorLeft.RunSpeedAndTime(-current, slowTime);  
    motorRight.RunSpeedAndTime(current, slowTime);
    Serial.print("current speed: ");
    Serial.println(current);
    delay(slowTime);
  }
  motorLeft.RunSpeed(carSpeed);
  motorRight.RunSpeed(carSpeed);
  delay(slowTime);
}


// post: return the distance it takes to stop this car, 
//              in millimeter
int stopSlowly() {
  int currentSpeedL = motorLeft.GetCurrentSpeed();
  int currentSpeedR = motorRight.GetCurrentSpeed();
  int startSpeed = min(currentSpeedL, currentSpeedR); 
  for (int i = startSpeed / 10; i >= 0 ; i--) {
    int current = i * 10;
    motorLeft.RunSpeedAndTime(-current, slowTime * 10);  
    motorRight.RunSpeedAndTime(current, slowTime * 10);
    Serial.print("current speed: ");
    Serial.println(current);
    delay(slowTime);
  }
  motorLeft.RunSpeed(0);
  motorRight.RunSpeed(0);
  delay(slowTime);
  return 0;
}

int stopSuddenly() {
  motorLeft.RunSpeed(0);
  motorRight.RunSpeed(0);
  delay(slowTime);
  return 0;
}

void serialFeedback() {
  Serial.print("Right: ");
  Serial.println(motorLeft.GetCurrentPosition());
  Serial.print("Left: ");
  Serial.println(motorRight.GetCurrentPosition());
}

