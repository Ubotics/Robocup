#include <SoftwareSerial.h>
#include <Wire.h>
#include <Arduino.h>
#include <Makeblock.h>

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");
}

void loop() // run over and over
{
  if (Serial1.available())
    Serial.write(Serial1.read());
}
