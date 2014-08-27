int previousError = 0;
void practiceWithLineFinder(boolean isOnPracticeMode) {
   if (isOnPracticeMode) {
       int sensorState = lineFinder.readSensors();
       switch(sensorState)
         {
            case S1_IN_S2_IN:  movePID(0);   previousError = 0; 
                               Serial.println("Sensor 1 and 2 are inside of black line");  break;
            case S1_IN_S2_OUT: movePID(1);   previousError = -1;
                               Serial.println("Sensor 2 is outside of black line");        break;
            case S1_OUT_S2_IN: movePID(-1);  previousError = 1;
                               Serial.println("Sensor 1 is outside of black line");        break;
            case S1_OUT_S2_OUT:movePID(previousError);    
                               Serial.println("Sensor 1 and 2 are outside of black line"); break;
            default:break;
          }
       delay(100);
   }
}
