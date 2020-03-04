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
  Serial.begin(250000); 
  motorInit();
  
  //Motor test functions
  //forward(7);
  //leftTurn(90);
  //rightTurn(90);
//  backward(6);
}

void loop() {
//Serial.setTimeout(50);
  //To clear the memory in the buffer
  memset(buffer, 0, sizeof buffer);

  // true if there are incoming bytes
   if (Serial.available() > 0)
    { 
    // e.g of string format to receive "PA: A2"
    Serial.readBytes(buffer, sizeof buffer);

    //converts char array to string for easier processing
    String inputString(buffer); 
    int inputString_length = inputString.length();
    char cmd = inputString.charAt(0);
    // only extract the number of grids to move if there is
    long distance_to_Move = inputString.substring(1,inputString_length-1).toInt();

    // commands received and process them    
    switch(cmd){
        // forward command
        case 'M' :
        case 'm' : 
                  forward(distance_to_Move);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // turn left
        case 'L' :
        case 'l' : 
                  leftTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // turn right
        case 'R' : 
        case 'r' : 
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // u-turn
        case 'U' : 
        case 'u' :
                  rightTurn(180);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // set mode of arduino to fastest path
        case 'F' :
        case 'f' :
                  // reset motor setting and PID
                  motorStart();
                  // so that it will know if need to check for obstacles depending if is fastest path or exploration run
                  notStartFastestPath = false;
                  // send back data to the sender that the robot is now ready to move
                  Serial.write("AE: started");
                  break;

        // for calibration command
        case 'C' :
        case 'c' :
                  // possible command from Algo PC: CF, CFL, CL (robot designed to be left hugging so won't have right wall to calibrate)
                  // F is front, L is left
                  for(uint8_t i=1; i<inputString.length(); i++)
                  {
                    
                    // algo PC let us know if left wall exist to allow calibration
                    if (inputString.charAt(i) == 'L')
                    {
                      // turn left to calibrate from left wall
                      leftTurn(90);
                      // calibrate angle and distance from the wall
                      CaliAngle();
                      // go back to facing front
                      rightTurn(90);
                    }
                    else
                    {
                      // calibrate angle and distance from the front wall since read 'F'
                      CaliAngle();
                    }
                  }
                  
                  break;

        // to send IR sensor reading of obstacles to everyone
        case 'S':
        case 's':
         getSensorData:
          // only send back the obstacle locations if is exploration run
          if(notStartFastestPath)
          {
            // get obstacle locations
            getObstacleLocations();
            // send obstacle locations to every other devices
            Serial.write(("AE: " + sensorResult).c_str());
          }
        }
   }

  //for obstacle avoidance checklist
//  if (setFlag)
//  {
//    setFlag = false;
//    
//    forwardTest();
//  }
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
  
