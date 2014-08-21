/***** Global Defines ****/ 

MePort port(PORT_1);
MePort headPort(PORT_7);
MeEncoderMotor motorLeft(0x8, 0x00);    //  Motor at PORT_2, slot1
MeEncoderMotor motorRight(0x8, 0x01);   //  motor at PORT_2, slot2

Servo arm;  // create servo object to control a servo; The Right side one, now is the blue holding motor
Servo hand;  // create servo object to control another servo
Servo head;
Servo head2;
int servo1pin =  port.pin1();//attaches the servo on PORT_1 SLOT1 to the servo object
int servo2pin =  port.pin2();//attaches the servo on PORT_1 SLOT2 to the servo object
int headServo =  headPort.pin1();
int headServo2=  headPort.pin2();

// These constants are for running
const int carSpeed = 30;
const int slowTime = 20;// in millisecond
const int processSpeed = 800;

// These two counters are for two motor;
int degreeCounterL = 0;
int degreeCounterR = 0;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

/// move states:
enum        {GRAB_ONE, GRAB_TWO, MOV_LEFT, MOV_RIGHT, MOV_FORWARD, MOV_BACK, MOV_ROTATE_L, MOV_ROTATE_R, MOV_AROUND, MOV_STOP};
const char* states[] = {"Grab_One", "Grab_Two", "Left", "Right", "Forward", "Back", "Rotate Left", "Rotate Right" , "Turn Around",  "Stop"};

