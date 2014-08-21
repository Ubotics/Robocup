//
/*
	the protocol would be something like (in both read and write):
	/DEVICE/PORT/SLOT/VALUE
*/

#define NC -1
#define NUM_QUERYLIST 10

#define SLOT1 0
#define SLOT2 1

#define PORT_1 				0x01
#define PORT_2 				0x02
#define PORT_3 				0x03
#define PORT_4 				0x04
#define PORT_5 				0x05
#define PORT_6 				0x06
#define PORT_7 				0x07
#define PORT_8 				0x08
#define M1     				0x09
#define M2     				0x0a

#define DEV_NULL "NULL"
#define DEV_ULTRASONIC "Ultrasonic Sensor"
#define DEV_PIR "PIR Motion Sensor"
#define DEV_BUTTON "Button"
#define DEV_LIMIT "Limit Switch"
#define DEV_POTENTIO "Potentiometer"
#define DEV_LINEFINDER "Line Finder"
#define DEV_SOUNDSENSOR "Sound Sensor"
#define DEV_TEMPERATURE "Temperature Sensor"
#define DEV_DCMOTOR "DC Motor"
#define DEV_SERVO "Servo Motor"
#define DEV_JOYSTICK "Joystick"
#define DEV_LED "Led"
#define DEV_IR "Ir"
#define DEV_DPORT_READ "R_DPORT"
#define DEV_APORT_READ "R_APORT"
#define DEV_DPORT_WRITE "W_DPORT"
#define DEV_APORT_WRITE "W_APORT"

typedef int (* FunPtr)(char port, char slot);
typedef struct{
  char port;
  char slot;
  char tmp;
  FunPtr fun;
  char * dev;
}QList;









