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
  //alignment();

  // turning manual calibration code
//  rightTurn(90);
//  delay(100);
//  rightTurn(90);
//  delay(100);
//  rightTurn(90);
//  delay(100);
//  rightTurn(90);
  
  //Motor test functions
  //forward(7);
  //leftTurn(90);
  //rightTurn(90);
//  backward(6);
}

void loop() {

  // true if there are incoming bytes
   if (Serial.available() > 0)
    { 
      // e.g of string format to receive "PA: A2"
      //Serial.readBytes(buffer, sizeof buffer);
      // read content from RPi (Serial.read() is processed by Arduino way faster than Serial.readbytes())
      char cmd = Serial.read();
  
      /*//converts char array to string for easier processing
      String inputString(buffer); 
      int inputString_length = inputString.length();
      char cmd = inputString.charAt(0);
      // only extract the number of grids to move if there is
      long distance_to_Move = inputString.substring(1,inputString_length-1).toInt();*/
  
      // commands received and process them    
      switch(cmd)
      {
        // forward 1 grid command
        case '1' :
        case 'M' :
        case 'm' :
                  forward(1);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 2 grids command
        case '2' :
                  forward(2);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 3 grids command
        case '3' :
                  forward(3);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 4 grids command
        case '4' :
                  forward(4);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 5 grids command
        case '5' :
                  forward(5);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 6 grids command
        case '6' :
                  forward(6);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 7 grids command
        case '7' :
                  forward(7);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 8 grids command
        case '8' :
                  forward(8);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;

        // forward 9 grids command
        case '9' :
                  forward(9);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;
  
        // turn left
        case 'L' :
        case 'l' : 
                  delay(100);
                  leftTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;
  
        // turn right
        case 'R' : 
        case 'r' : 
                  delay(100);
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                  break;
  
        // u-turn
        case 'U' : 
        case 'u' :
                  // turning 180 degrees have some issues. Seperate the turns to 90 90 is more accurate
                  delay(100);
                  rightTurn(90);
                  delay(100);
                  rightTurn(90);
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
  
        // for calibrate front and left
        case 'A' :
        case 'a' :
                  // calibrate angle and distance from the front wall
                  CaliAngle();
                  // turn left to calibrate from left wall
                  leftTurn(90);
                  delay(100);
                  // calibrate angle and distance from the left wall
                  CaliAngle();
                  // go back to facing front
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                    
                  break;

        // for calibrate left
        case 'B' :
        case 'b' :
                  // turn left to calibrate from left wall
                  leftTurn(90);
                  delay(100);
                  // calibrate angle and distance from the left wall
                  CaliAngle();
                  // go back to facing front
                  rightTurn(90);
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                    
                  break;

        // for calibrate front
        case 'C' :
        case 'c' :
                  // calibrate angle and distance from the front wall
                  CaliAngle();
                  // to send IR sensor reading of obstacles to everyone
                  goto getSensorData;
                    
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
  
