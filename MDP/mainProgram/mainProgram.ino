#include <DualVNH5019MotorShield.h>
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include <RunningMedian.h>
#include <SharpIR.h>

DualVNH5019MotorShield md;

volatile bool setFlag = true;
volatile bool notStartFastestPath = true;
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
////To clear the memory in the buffer
//memset(buffer, 0, sizeof buffer);

//  // true if there are incoming bytes
//   if (Serial.available() > 0)
//    { 
//    // e.g of string format to receive "PA: A2"
//    Serial.readBytes(buffer, sizeof buffer);
//
//    //converts char array to string for easier processing
//    String inputString(buffer); 
//    int inputString_length = inputString.length();
//    char cmd = inputString.charAt(4);
//    // only extract the number of grids to move if there is
//    long distance_to_Move = inputString.substring(1,inputString_length-2).toInt();
//
//    // commands received and process them    
//    switch(cmd){
//        // forward command
//        case 'W' :
//        case 'w' : 
//                  forward(distance_to_Move);
//                  // only send back the obstacle locations if is exploration run
//                  if (notStartFastestPath)
//                    goto sendObstacleLoc;
//                  break;
//
//        // turn left
//        case 'A' :
//        case 'a' : 
//                  leftTurn(90);
//                  // only send back the obstacle locations if is exploration run
//                  if (notStartFastestPath)
//                    goto sendObstacleLoc;
//                  break;
//
//        // turn right
//        case 'D' : 
//        case 'd' : 
//                  rightTurn(90);
//                  // only send back the obstacle locations if is exploration run
//                  if (notStartFastestPath)
//                    goto sendObstacleLoc;
//                  break;
//
//        // move backwards
//        case 's' : 
//                  backward(distance_to_Move);
//                  break;
//
//        // set mode of arduino to fastest path
//        case 'S':
//                  // reset motor setting and PID
//                  motorStart();
//                  // so that it will know if need to check for obstacles depending if is fastest path or exploration run
//                  notStartFastestPath = false;
//                  // send back data to the sender that the robot is now ready to move
//                  Serial.write(inputString.charAt(1) + inputString.charAt(0) + ": started");
//                  break;
//
//        // for calibration command
//        case 'C':
//                  break;
//
//        // get obstacle locations
//        default:
//        sendObstacleLoc:
//                  // get obstacle locations
//                  getObstacleLocations();
//                  // send obstacle locations to every other devices
//                  Serial.write(("AE: " + sensorResult).c_str());
//        }  
//   }

// forward(15);
//  if(PS2_value == 1)
//  {
//    motorStop();
//    delay(1000);
//    }
//
// for obstacle avoidance checklist
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
  
