volatile double rpm_L; //rpm of left motor
volatile double rpm_R; //rpm of right motor
volatile double step_L; //number of steps taken by the left motor
volatile double step_R; //number of steps taken by the right motor
volatile double error; //difference between left and right steps

volatile const int SPEED =  320; //Set the speed of the motor

// PID Parameters
// PID values for obstacle avoidance checklist because interupt not working well due to delay checking the sensor: Kp=6, Ki=0, Kd=0.008
volatile double Kp=19, Ki=0, Kd=0; //Set PID values //Previous adjusted PID values: Kp=23, Ki=80, Kd=0.02 (When weight is one RPi and 2 powerbanks given PID values: Kp=30, Ki=80, Kd=0.02)
volatile double Kp1=13, Ki1=0, Kd1=0;
volatile double Output = 0; // Adjusted speed

// Create PID Instance
PID myPID(&rpm_R, &Output, &rpm_L, Kp, Ki, Kd, DIRECT);
PID myPID1(&rpm_R, &Output, &rpm_L, Kp1, Ki1, Kd1, DIRECT);
/*Where parameters of the function are:
myPID(input(rpm), output(speed), setpoint(rpm), proportional value, intergral value, derative value, motor direction)*/

void forward(double gridNumber){
  
  double totalSteps = totalRegularSteps(getFinalSteps(gridNumber));

  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(i, i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) 
  {
    computeRPM();
    if (gridNumber != 1)
      myPID.Compute();
    else
      myPID1.Compute();
    error = step_L - step_R;
    double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
    md.setSpeeds(SPEED + adjust + Output + addSPEED_R, SPEED - adjust);
  }

  motorStop();
  //delay(10);
  }

void backward(double gridNumber){
  // seperate number of ticks cause of the weight unevenness
  double totalSteps = totalRegularSteps(gridNumber * 8.2);
  
  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(-i, -i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps )
  {
    //Serial.print(step_R); // debug
    computeRPM();
    if (gridNumber != 1)
      myPID.Compute();
    else
      myPID1.Compute();
    error = step_L - step_R;
    double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
    md.setSpeeds(-SPEED - adjust - Output, -SPEED + adjust);
  }

  motorStop();
  //delay(10);
  }

void leftTurn(double degree)
{
  double totalSteps = totalAngularSteps_L(degree);

  motorStart();
  for(int i=0; i<=SPEED; i++)
  {
    md.setSpeeds(i, -i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps )
  {
    computeRPM();
    myPID.Compute();
    error = step_L - step_R;
    double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
    md.setSpeeds(SPEED + adjust + Output, -SPEED + adjust);
  }

  motorStop();
  delay(75);
  }

void rightTurn(double degree)
{
  double totalSteps = totalAngularSteps_R(degree);

  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(-i, i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps )
  {
    computeRPM();
    myPID.Compute();
    error = step_L - step_R;
    double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
    md.setSpeeds(-SPEED - adjust - Output, SPEED - adjust);
  }

  motorStop();
  delay(75);
}

// calibrate robot to correct angle from the wall of front using front right (PS1) and front left (PS3) sensors
// if the angle is correct, both the readings shouldn't deviate much
double CaliAngle(bool firstTime, bool leftWall)
{
  bool firstCounter = true;
  double distDiff;
  double distDiff_retVal = 0;
  int count = 0;
  int calSPEED = 50;

  // range that sensor is still roughly stable hence can use those values to calibrate
  while (getDistance(3) < 30 && getDistance(1) < 30)
  {
    // so won't have infinite loop as it will never be fully perfect
    if (count == 180)
    {
      // stop robot moving
      motorStop();

      // if got wall on the left of the robot, turn right a bit 1st
      if (leftWall)
        rightTurn(10);
      else
        leftTurn(10);

      /*// beta testing
      // check if it didn't break out of the loop after turning a bit before then turn to the other side instead
      if (firstCounter)
      {
        // if got wall on the left of the robot, turn right a bit 1st
        if (leftWall)
          rightTurn(10);
        else
          leftTurn(10);

        firstCounter = false;
      }
      // incase there is left wall and right wall then it gets stuck facing the corner of front wall and right wall while was attempting to turn right more
      else if (leftWall)
      {
        leftTurn(90);
        CaliAngle(true, false);
        rightTurn(90);
      }
      */

      count = 0;
    }

    // if different is a lot, means the robot is not perpenticular to the wall
    distDiff = getDistance(3) - getDistance(1);

    // assign the original off value to be used for dynamic initial calibration
    if(distDiff > distDiff_retVal)
      distDiff_retVal = distDiff;

    // too tilted to the left, turn to the right
    if (distDiff >= 0.05) 
      md.setSpeeds(-calSPEED, calSPEED);
    // too titled to the right, turn to the left
    else if (distDiff <= -0.05)
      md.setSpeeds(calSPEED, -calSPEED);
    // correct angle
    else
    {
      // stop the robot from moving
      motorStop();
      
      // calibrate angle again after moving (useful if robot was too far from the wall when calibrating angle causing angle calibration to be off)
      if (firstTime)
        caliDist(leftWall);
      break;
    }
  }

  return distDiff_retVal;
}

// Calibrate robot to targetDist from the wall of front using front right (PS1) and front left (PS3) sensors.
// Uses PS1 and PS3 cause there if those wall/obstacle exist for PS1 and PS3 to calibrate angle, 
//    it can too for distance calibration which there maybe be no wall/obstacle at PS2's direction
void caliDist(bool leftWall)
{
  bool tooMuch = false;
  bool tooLittle = false;
  bool overFlow = false;
  int SPEEDL = 100;
  int SPEEDR = 100;
  int count = 0;

  // infinite WHILE loop till it's completed
  while (1)
  {
    if(count == 1000 || overFlow)
      break;
    
    // if too far off, move closer
    if (getDistance(3) > 4.9 || getDistance(1) > 4.9)
    {
      md.setSpeeds(SPEEDR, SPEEDL);

      // helps prevent robot moving back and forward too many times till count == 1000 if the speed of the robot is too high preventing the robot from reaching the acceptable threshold. 
      if (tooLittle)
        overFlow = true;
      else
        tooMuch = true;
    }
    // if too close, reverse
    else if (getDistance(3) < 4.0 || getDistance(1) < 4.0)
    {
      md.setSpeeds(-SPEEDR, -SPEEDL);

      // helps prevent robot moving back and forward too many times till count == 1000 if the speed of the robot is too high preventing the robot from reaching the acceptable threshold. 
      if (tooMuch)
        overFlow = true;
      else
        tooLittle = true;
    }
    // within acceptable range, stop calibrating
    else 
      break;

    ++count;
  }

  // stop the robot from moving
  motorStop();

  // calibrate angle again after moving (useful if robot was too far from the wall when calibrating angle causing angle calibration to be off)
  CaliAngle(false, leftWall);
}

// alignment of robot to the starting position
void alignment()
{
  delay(1500);
  rightTurn(180);
  delay(100);
  CaliAngle(true, false);
  rightTurn(90);
  delay(100);
  CaliAngle(true, true);
  rightTurn(90);
  delay(200);
}


//////////////////////////////////////////////////// Obstacle avoidance checklist ////////////////////////////////////////////////////

// for obstacle avoidance checklist
void avoid(){
  PS2_obstacleDetection();

  if(PS2_value == 2)
  {
    if (secondAvoid)
      return;
    else
    {
      motorStop();
      delay(100);
      rightTurnTest(45);
      delay(100);
      forward(3);
      delay(100);
      leftTurnTest(45);
      delay(100);
      forward(5);
      delay(100);
      leftTurnTest(45);
      delay(100);
      forward(3);
      delay(100);
      rightTurnTest(45);
      motorStop();

      // hardcode stop before the end of the arena
      secondAvoid = true;
      }
  }
}

// for obstacle avoidance checklist
void forwardTest(){
  
  double totalSteps = totalRegularSteps(fifteen_grid);

  motorStart();
  
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(i, i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps )
  {
    avoid();
    if(PS2_value == 2 && secondAvoid)
      break;
    computeRPM();
    myPID.Compute();
    error = step_L - step_R;
    double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
    md.setSpeeds(SPEED + adjust + Output, SPEED - adjust);
  }

  motorStop();
  delay(10);
  }

void backwardTest(){
  double totalSteps = totalRegularSteps(fifteen_grid);

  motorStart();
  
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(-i, -i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) {
  computeRPM();
  myPID.Compute();
  error = step_L - step_R;
  double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
  md.setSpeeds(-SPEED - adjust - Output, -SPEED + adjust);
  }

  motorStop();
  delay(10);
  }

// for obstacle avoidance checklist
void leftTurnTest(double degree){
  
  double totalSteps = totalAngularSteps_L(degree);

  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(i, -i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) {
  computeRPM();
  myPID.Compute();
  error = step_L - step_R;
  double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
  md.setSpeeds(SPEED + adjust + Output, -SPEED + adjust);
  }

  motorStop();
  delay(10);
}

// for obstacle avoidance checklist
void rightTurnTest(double degree){
  double totalSteps = totalAngularSteps_R(degree);

  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(-i, i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) {
  computeRPM();
  myPID.Compute();
  error = step_L - step_R;
  double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
  md.setSpeeds(-SPEED - adjust - Output, SPEED - adjust);
  }

  motorStop();
  delay(10);
}
