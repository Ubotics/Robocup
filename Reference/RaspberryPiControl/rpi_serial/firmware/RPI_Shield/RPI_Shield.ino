#include <SoftwareSerial.h>
#include <Wire.h>
#include <String.h>
#include <OneWire.h>
#include "MKProtocol.h"
#include "Servo.h"
#include "WS2812.h"


#define NOP __asm__("nop\n\t")
#define HIGH0 *P_RGB = PINON;*P_RGB = PINOFF;
#define HIGH1 *P_RGB = PINON;NOP;NOP;NOP;NOP;*P_RGB = PINOFF;

#define buzzerOn()  DDRE |= 0x04,PORTE |= B00000100
#define buzzerOff() DDRE |= 0x04,PORTE &= B11111011


// Platform specific I/O definitions
#define FALSE 0
#define TRUE  1

#if defined(__AVR__)
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_ASM asm("r30")
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))
#endif

#if defined(__AVR_ATmega32U4__) //MeBaseBoard use ATmega32U4 as MCU
unsigned char mePort[11][2] = {{NC, NC}, {11, A8}, {13, A11}, {A10, A9}, {1, 0},
    {MISO, SCK}, {A0, A1}, {A2, A3}, {A4, A5}, {6, 7}, {5, 4}
};
#else // else ATmega328
unsigned char mePort[11][2] = {{NC, NC}, {11, 10}, {3, 9}, {12, 13}, {8, 2},
    {NC, NC}, {A2, A3}, {NC, A1}, {NC, A0}, {6, 7}, {5, 4}
};
#endif

#define dbgPort Serial1

QList qlist[NUM_QUERYLIST];
Servo servo;
SoftwareSerial * ir;

void doPortPinmode(char port,char slot,char mode)
{
  char pin = mePort[port][slot];
  pinMode(pin,mode);
  
}

void doDWrite(char port,char slot,char value)
{
  char pin = mePort[port][slot];
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
  
}

int doDRead(char port, char slot)
{
  char pin = mePort[port][slot];
  pinMode(pin, INPUT);
  return digitalRead(pin);
}

void doAWrite(char port,char slot,char value)
{
  char pin = mePort[port][slot];
  analogWrite(pin, value);
}

int doARead(char port, char slot)
{
  char pin = mePort[port][slot];
  return analogRead(pin);
}

void doServoRun(char port, char slot, int pos)
{
  char pin;
  pin = mePort[port][slot];
  servo.attach(pin);
  servo.write(pos);
}

void doDcRun(char port, int speed)
{
  char dpin,apin;
  dpin = mePort[port][1]; // digit pin
  apin = mePort[port][0]; // analog pin

  speed = speed > 255 ? 255 : speed;
  speed = speed < -255 ? -255 : speed;

  if(speed >= 0) {
    pinMode(dpin, OUTPUT);
    digitalWrite(dpin, HIGH);
    analogWrite(apin, speed);  
  } else {
    pinMode(dpin, OUTPUT);
    digitalWrite(dpin, LOW);
    analogWrite(apin, -speed);  
  }
}

int doDistance(char port, char slot)
{
  char pin;
  pin  = mePort[port][1];
  pinMode(pin, OUTPUT);digitalWrite(pin, LOW);
  delayMicroseconds(2);
  pinMode(pin, OUTPUT);digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  pinMode(pin, OUTPUT);digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  long distance = pulseIn(pin, HIGH); 
  delay(20);
  return ((distance / 29) >> 1);
}


/** ws2812 related **/
uint8_t color[3]={0,0,0};
uint16_t lastColor[8];
void doRgbRun(char port, char slot, unsigned int color565)
{
  volatile uint8_t *P_RGB; // port register of ws2812
  uint8_t B_RGB;  // register mask of ws2812 output pin
  uint8_t PINON,PINOFF;
  char a;
  char i,j,k;
  char pin  = mePort[port][slot];
  color[1] = (color565>>11)* 255 / 31; // red
  color[0] = ((color565>>5)&0x3f)* 255 / 63; // green
  color[2] = (color565&0x1f)* 255 / 31; // blue

  if(color565==lastColor[port]){
    return;
  }
  lastColor[port] = color565;
  pinMode(pin, OUTPUT);
  //digitalWrite(pin, LOW);
  P_RGB = portOutputRegister(digitalPinToPort(pin));
  B_RGB = digitalPinToBitMask(pin);
  PINON = *P_RGB | B_RGB;
  PINOFF = *P_RGB & ~B_RGB;
  cli();
  for(i=0;i<16;i++){ // the max pixel length 16
    // send 24bit rgb color
    for(j=0;j<3;j++){
      a = color[j];
      for(k=0;k<8;k++){
        if(a&0x80){
          HIGH1;
        }else{
          HIGH0;
        }
        a<<=1;
      }
    }
  }
  sei();
}


/** DS18B20 related **/
// a easy 18b20 driver, only support single sensor on one-wire, but small and easy to understand
#define DS18B20_HIGH(pin) digitalWrite(pin,LOW);delayMicroseconds(2);digitalWrite(pin,HIGH);delayMicroseconds(62);
#define DS18B20_LOW(pin) digitalWrite(pin,LOW);delayMicroseconds(55);digitalWrite(pin,HIGH);delayMicroseconds(10);

void ds18b20_init(int pin)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delayMicroseconds(500);
  digitalWrite(pin,HIGH);
  pinMode(pin,INPUT);
  int dur = pulseIn(pin, LOW, 1000);// typically around 120
  //Serial.printf("dur= %d\n",dur);
}

void ds18b20_write(unsigned char v, int pin)
{
  pinMode(pin,OUTPUT);
  int i;
  for(i=0;i<8;i++){
    if(v&0x01){
      DS18B20_HIGH(pin);
    }else{
      DS18B20_LOW(pin);    
    }
    v>>=1;
  }
}

unsigned char ds18b20_readbit(int pin)
{
  unsigned char ret=0;
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delayMicroseconds(3);
  pinMode(pin,INPUT);
  delayMicroseconds(10);
  if(digitalRead(pin))
    ret = 1;
  delayMicroseconds(50);
  return ret;
}

unsigned char ds18b20_read(int pin)
{
  int i;
  unsigned char dat=0;
  for(i=0;i<8;i++){
    dat+=(ds18b20_readbit(pin)<<i);
  }
  return dat;
}

int doTemperature(char port, char slot)
{
  char pin;
  int t=0;
  pin  = mePort[port][slot];
  cli();
  ds18b20_init(pin);
  ds18b20_write(0xcc,pin);
  ds18b20_write(0x44,pin);
  sei();
  delay(2);
  cli();
  ds18b20_init(pin);
  ds18b20_write(0xcc,pin);
  ds18b20_write(0xbe,pin);
  t=ds18b20_read(pin); // LSB  
  t|=(ds18b20_read(pin)<<8); // MSB 
  sei();  
  //Serial.printf("doTemperature %d\n",t);
  return t;
}

int doInfraredReceiver(char port, char slot)
{
  char cmd=-1;
  if(ir->available()){
    cmd = ir->read();
    if(cmd==0) cmd=-1; // trip softserial
  }
  return cmd;
}
/*--------------------- END -------------------------*/

char buff[64];
char buffIndex=0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200); // bluetooth serial
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(dbgPort.available()){
    char c = dbgPort.read();
    Serial.print(c);
    if(c=='\n'){
      parseCommand(buff);
      memset(buff,0,64);
      buffIndex=0;
    }else{
      buff[buffIndex++]=c;
      if(buffIndex>60){
        memset(buff,0,64);
        buffIndex=0;
      }
    }
  }
  // todo: use timer schedule
  doQuery();
}


void parseCommand(char * cmdBuff)
{
  char * tmp;
  char * a[10];
  char* slashStart;
  // split the input string by '/'
  slashStart = strstr(cmdBuff,"/"); // find the start position of /
  if(!slashStart) return;
  //Serial.printf("slashPos=%s\n",slashStart);
  a[0] = strtok_r(slashStart, "/", &tmp);
  //Serial.printf("%s\n",a[0]);
  int n = 1;
  while((a[n] = strtok_r(0, "/", &tmp)) != 0){
    //Serial.printf("%d %s\n",n,a[n]);
    n++;
  }
  // /DEV/VALUE/......
  if(0==strcmp(a[0], DEV_DCMOTOR)){
    doDcRun(atoi(a[1]),atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_SERVO)){
    doServoRun(atoi(a[1]),atoi(a[2]),atoi(a[3]));
  }else if(0==strcmp(a[0], DEV_JOYSTICK)){
    // ultilize your own joystick control below
    Serial.print("joystick");
  }else if(0==strcmp(a[0], DEV_LED)){
    doRgbRun(atoi(a[1]), atoi(a[2]), atoi(a[3]));
  }else if(0==strcmp(a[0], DEV_APORT_WRITE)){
    doAWrite(atoi(a[1]), atoi(a[2]), atoi(a[3]));
  }else if(0==strcmp(a[0], DEV_DPORT_WRITE)){
    doDWrite(atoi(a[1]), atoi(a[2]), atoi(a[3]));
  }
  // query related functions
  else if(0==strcmp(a[0], DEV_ULTRASONIC)){
    appendQueryList(DEV_ULTRASONIC,doDistance,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_TEMPERATURE)){
    appendQueryList(DEV_TEMPERATURE,doTemperature,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_SOUNDSENSOR)){
    appendQueryList(DEV_SOUNDSENSOR,doARead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_POTENTIO)){
    appendQueryList(DEV_POTENTIO,doARead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_PIR)){
    appendQueryList(DEV_PIR,doDRead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_BUTTON)){
    appendQueryList(DEV_BUTTON,doARead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_LIMIT)){
    appendQueryList(DEV_LIMIT,doDRead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_DPORT_READ)){
    appendQueryList(DEV_DPORT_READ,doDRead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_APORT_READ)){
    appendQueryList(DEV_APORT_READ,doARead,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_IR)){
    char port;
    port = atoi(a[1]);
    // InfraredReceiver only valid on port5
    if(port!=PORT_5){
      return;
    }
    free(ir);
    ir = new SoftwareSerial(mePort[port][1],mePort[port][0]);
    ir->begin(9600);
    appendQueryList(DEV_IR,doInfraredReceiver,atoi(a[1]), atoi(a[2]));
  }else if(0==strcmp(a[0], DEV_NULL)){
    removeQueryList(atoi(a[1]), atoi(a[2]));
  }
}

void sendCommand(char * dev, int port, int slot, int value)
{
  dbgPort.print(F("/"));  dbgPort.print(dev);
  dbgPort.print(F("/"));  dbgPort.print(port);
  dbgPort.print(F("/"));  dbgPort.print(slot);
  dbgPort.print(F("/"));  dbgPort.println(value);
}

void doQuery()
{
  int i;
  for(i=0;i<NUM_QUERYLIST;i++){
    if(qlist[i].dev!=0){
      int ret;
      ret = qlist[i].fun(qlist[i].port,qlist[i].slot);
      // dread only return changed
      if(qlist[i].fun == doDRead){
       if(qlist[i].tmp == ret) continue;
       qlist[i].tmp = ret;
      }
      if(ret<0) continue;
      
      sendCommand(qlist[i].dev,qlist[i].port,qlist[i].slot,ret);
    }
  }
}

int appendQueryList(char * device, FunPtr queryfun, char port, char slot)
{
  int i;
  for(i=0;i<NUM_QUERYLIST;i++){
    if(qlist[i].dev == 0){
        qlist[i].fun = queryfun;
        qlist[i].port = port;
        qlist[i].slot = slot;
        qlist[i].dev = device;
        qlist[i].tmp = 0;
        return i;
    }
  }
  return -1;
}

int removeQueryList(char port, char slot)
{
  int i;
  for(i=0;i<NUM_QUERYLIST;i++){
    if(qlist[i].port == port && qlist[i].slot == slot){
        qlist[i].fun = 0;
        qlist[i].port = 0;
        qlist[i].slot = 0;
        qlist[i].dev = 0;
        return i;
    }
  }
  return -1;
}

















