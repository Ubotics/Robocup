//  Encoder Library会是之后控制前进方向最重要的库，需要迅速掌握

#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

//  其特点如下：
//  1. Constructer，无需创建MePort Object, 一个 
        MeEncoderMotor motorLeft(0x8, 0x00); 
        MeEncoderMotor motorRight(0x8, 0x01);
        // 即可完成创建，0x00 and 0x01 are different slots，0x8只是一个固定的参数
        // 可插在Port 1 or 2
    
//  2. Setup部分，和Serial一样，直接begin就可以
        void setup() {
            motorLeft.begin();
            motorRight.begin();
//            Serial.begin(9600)；
//            Serial1.begin(115200); // Communicate to Raspberry Pi
        }
        
//  3. Loop 部分可以调用的函数超强，是步进电机加舵机所有能力，
//     能走角度，步数，返回速度，甚至能直接返回绝对坐标！
        /*
        boolean Reset();
        
        // 这两个是舵机的能力
        boolean Move(float angle, float speed);
        boolean MoveTo(float angle, float speed);
        
        // 这个是步进电机的能力
        boolean RunTurns(float turns, float speed);
        
        // 控速行驶
        boolean RunSpeed(float speed);
        boolean RunSpeedAndTime(float speed, float time);
        
        // 返回速度
        float GetCurrentSpeed();
        
        // 返回位置！
        float GetCurrentPosition();
        */
        void loop() {
            //if (Serial1.read() == CaseOne) {
            //    float PositionNow = GetCurrentPosition();
            //    if (PositionNow != DestinationOne) {
                    motorLeft.RunSpeedAndTime(100, 5);
                    motorRight.RunSpeedAndTime(100, 5);
                    delay(5000);
            //    }
            //}
        }
