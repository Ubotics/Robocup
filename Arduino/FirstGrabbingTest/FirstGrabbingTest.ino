#include <Makeblock.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h> // include the Servo library for grabbing
#include <PID_v1.h>
#include "robotDefines.h"

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    while (!Serial) {;}
    while (!Serial1) {;}
    Serial.println("Goodnight moon!");
    moveBegin();
    servoBegin();
}

int distance = 0;
void loop()
{
    servoHold();
    delay(processSpeed*4);
    startSlowly();
    moveForward(740);
    delay(processSpeed * 10);
    startSlowly();
    moveForward(220);
    stopSlowly();
    delay(processSpeed * 5);
    remoteService();
    delay(processSpeed);
    remoteService();
    delay(processSpeed);
}

