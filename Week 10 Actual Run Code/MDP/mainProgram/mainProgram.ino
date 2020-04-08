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
  Serial.begin(1000000); 
  motorInit();

  // align the robot to start position properly
 alignment();
 // for d1ynamic angular calibration
 dynamicAngularCalibration();
}

void loop() 
{
  // true if there are incoming byte(s)
   if (Serial.available() > 0)
    { 
      // read content from RPi (Serial.read() is processed by Arduino way faster than Serial.readbytes())
      char cmd = Serial.read();
  
      // commands received and process them    
      switch(cmd)
      {
        // forward 1 grid command
        case '1' :
        case 'M' :
        case 'm' :
                  forward(1);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 2 grids command
        case '2' :
                  forward(2);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 3 grids command
        case '3' :
                  forward(3);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 4 grids command
        case '4' :
                  forward(4);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 5 grids command
        case '5' :
                  forward(5);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 6 grids command
        case '6' :
                  forward(6);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 7 grids command
        case '7' :
                  forward(7);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 8 grids command
        case '8' :
                  forward(8);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // forward 9 grids command
        case '9' :
                  forward(9);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                      getSensorData();
                  break;

        // keep moving forward while sending to Algo PC sensor data every 1 grid after moving. Only stop when have at least one obstacle 1 grid from it
        case 'N' :
        case 'n' :
                  while(1)
                  {
                    forward(1);
                    delay(10);
                    // to send IR sensor reading of obstacles to everyone
                    if (notStartFastestPath)
                      getSensorData();

                    // check if there is any obstacle infront of the robot (at 1 grid) using PS1, PS2, PS3 sensors. If have, stop moving forward and wait for commands from Algorithm PC.
                    if (sensorResult.charAt(0) == '1' || sensorResult.charAt(2) == '1' || sensorResult.charAt(4) == '1')
                      break;
                  }

        // move backwards 1 grid command
        case 'D' : 
        case 'd' :
                  backward(1);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                    getSensorData();
                  break;
  
        // turn left
        case 'L' :
        case 'l' : 
                  delay(75);
                  leftTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                    getSensorData();
                  break;
  
        // turn right
        case 'R' : 
        case 'r' : 
                  delay(75);
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                    getSensorData();
                  break;
  
        // u-turn
        case 'U' : 
        case 'u' :
                  // turning 180 degrees have some issues. Seperate the turns to 90 90 is more accurate
                  delay(75);
                  rightTurn(90);
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                    getSensorData();
                  break;
  
        // set mode of arduino to fastest path
        case 'F' :
        case 'f' :
                  // reset motor setting and PID
                  motorStart();
                  // so that it will know if need to check for obstacles depending if is fastest path or exploration run
                  notStartFastestPath = !notStartFastestPath;

                  // to send IR sensor reading of obstacles to everyone
                  if (notStartFastestPath)
                    getSensorData();
                  break;
  
        // for calibrate front and left
        case 'A' :
        case 'a' :
                  // calibrate angle and distance from the front wall
                  CaliAngle(true, true);
                  // turn left to calibrate from left wall
                  leftTurn(90);
                  // calibrate angle and distance from the left wall
                  CaliAngle(true, false);
                  // go back to facing front
                  rightTurn(90);
                  // calibrate angle and distance from the front wall
                  CaliAngle(true, true);
                  // get sensor reading and send to Algorithm PC
                  if (notStartFastestPath)
                    getSensorData();
                  //delay(100);
                  break;

        // for calibrate left
        case 'B' :
        case 'b' :
                  // turn left to calibrate from left wall
                  leftTurn(90);
                  // calibrate angle and distance from the left wall
                  CaliAngle(true, false);
                  // go back to facing front
                  rightTurn(90);
                  // get sensor reading and send to Algorithm PC
                  if (notStartFastestPath)
                    getSensorData();
                  //delay(100);
                  break;

        // for calibrate front
        case 'C' :
        case 'c' :
                  // calibrate angle and distance from the front wall
                  CaliAngle(true, false);
                  // get sensor reading and send to Algorithm PC
                  if (notStartFastestPath)
                    getSensorData();
                  //delay(100);
                    
                  break;

        // for calibrate front and right
        case 'G' :
        case 'g' :
                  // calibrate angle and distance from the front wall. Assume left wall does not exist
                  CaliAngle(true, false);
                  rightTurn(90);
                  // calibrate angle and distance from the front wall. After turning, left wall exist
                  CaliAngle(true, true);
                  leftTurn(90);
                  //delay(100);
                  break;

        // for calibrate right
        case 'E' :
        case 'e' :
                  rightTurn(90);
                  // calibrate angle and distance from the front wall
                  CaliAngle(true, false);
                  leftTurn(90);
                  //delay(100);
                  break;

        // for starting position alignment of the robot
        case 'P' :
        case 'p' :
                  alignment();
                  // for dynamic angular calibration
                  dynamicAngularCalibration();
                  // to send IR sensor reading of obstacles to everyone
                  getSensorData();
                  delay(100);
                  break;
  
        // to send IR sensor reading of obstacles to everyone
        case 'S':
        case 's':
                getSensorData();
                break;
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

void getSensorData()
{
  // get obstacle locations
  getObstacleLocations();
  // send obstacle locations to every other devices
  Serial.write(("AE: " + sensorResult).c_str());
}
  
