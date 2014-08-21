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

void loop()
{
    servoHold();
    delay(processSpeed*3);
    startSlowly();
    // moveForward(740);
    while (!Serial1.available() ) {
       startSlowly();
       moveForward(30);
       delay(200);
    }
    int cmd = Serial1.read();
    Serial.println(cmd);
    processCommand(cmd);   
    delay(processSpeed * 10);
    remoteService();
    stopSuddenly();
    delay(processSpeed * 100);
    //moveForward(270);
    //delay(processSpeed * 4);
    // grabOneTube();
    // delay(processSpeed);
    // stopSlowly();
    // remoteService(); 
}


// Task 1, Steps by Steps
// 0. rise the arm from the beginning
// 1. move straight to center with PID, 20cm beforehead;
// 2. Upper camera helps to get center, using similarity and shaking the camera;
// 3. Turn Left 90;
// 4. move straight to first grabbing point with PID, 20 cm beforehead;
// 5. Upper camera helps wit similarity and stop
// 6. start to sense the color
// 7. grab, rise and hold
// 8. Raspberry Pi record the color
// 9. turn 180 degree, start PID and similarity.
// 10. Move to center, 
// 11. turn right 45 degree;
// 12. go straight to the saving point;
// 13. unload, go back 
// 14. turn 180 degree
// 15. go to center
// 16. repeat 4. to 15 once;
// 17. repeat 4. to 10.

// 18. based on the E color, turn certain degree
// 19. go straight to the target 
// 20. put to the target
// 21. finish the rest of the two



