#include <DualVNH5019MotorShield.h>
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include <RunningMedian.h>
#include <SharpIR.h>
DualVNH5019MotorShield md;
volatile bool setFlag = true;
String testOfTextFromRPi;
String sensorResult;

char buffer[25];
int PS2_value = -1;
bool secondAvoid = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  motorInit();
  //Motor test functions
//  forward(15);
 // leftTurn(720);
//  rightTurn(90);
//  backward(6);
}

void loop() {
//Serial.setTimeout(50);
//memset(buffer, 0, sizeof buffer);
//   if (Serial.available() > 0) { //Wait for input
//    Serial.readBytes(buffer, 25);
//
//    String inputString = Serial.readString(); 
//    int inputString_length = inputString.length();
//    char cmd = inputString.charAt(0);
//    long distance_to_Move = inputString.substring(1,inputString_length-1).toInt();
//    //double distance_to_Move_1 = getDistanceAccurateCM(distance_to_Move);
//
//        switch(cmd){
//        case 'W' :
//        case 'w' : 
//                  break;
//        case 'A' :
//        case 'a' : 
//                  break;
//        case 'D' : 
//        case 'd' : 
//                  break;
//        case 'S' : 
//        case 's' : 
//                  break;
//        }  
//   }
  //calibrateSensor();
// forward(15);
//  if(PS2_value == 1)
//  {
//    motorStop();
//    delay(1000);
//    }
//
  if (setFlag)
  {
    setFlag = false;
    
    forwardTest();
  }
}

  // Example of receiving data from RPi and to RPi
  /*
  if(strcmp(buffer, "Hello\n") == 0)
    Serial.write("Hello RPi!\n");
  else if(strcmp(buffer, "") > 0)
    Serial.write("Not empty string!\n");
  else if(strcmp(buffer, "") == 0)
    Serial.write("Empty string!\n");
  */
  
