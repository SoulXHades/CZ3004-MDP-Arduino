volatile double rpm_L; //rpm of left motor
volatile double rpm_R; //rpm of right motor
volatile double step_L; //number of steps taken by the left motor
volatile double step_R; //number of steps taken by the right motor
volatile double error; //difference between left and right steps

volatile const int SPEED =  250; //Set the speed of the motor

// PID Parameters
// PID values for obstacle avoidance checklist because interupt not working well due to delay checking the sensor: Kp=6, Ki=0, Kd=0.008
volatile double Kp=23, Ki=80, Kd=0.02; //Set PID values //Previous adjusted PID values: Kp=23, Ki=80, Kd=0.02
volatile double Output = 0; // Adjusted speed

// Create PID Instance
PID myPID(&rpm_R, &Output, &rpm_L, Kp, Ki, Kd, DIRECT);
/*Where parameters of the function are:
myPID(input(rpm), output(speed), setpoint(rpm), proportional value, intergral value, derative value, motor direction)*/

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

void forward(double gridNumber){
  
  double totalSteps = totalRegularSteps(getFinalSteps(gridNumber));

  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(i, i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) {
  computeRPM();
  myPID.Compute();
  error = step_L - step_R;
  double adjust = (error!=0) ? (error>0 ? 1 : -1) : 0;
  md.setSpeeds(SPEED + adjust + Output, SPEED - adjust);
  }

  motorStop();
  //delay(10);
  }

void backward(double gridNumber){
  double totalSteps = totalRegularSteps(getFinalSteps(gridNumber));
  
  motorStart();
  for(int i=0; i<=SPEED; i++){
    md.setSpeeds(-i, -i);
  }
  
  while (step_R <= totalSteps || step_L <= totalSteps ) {
    Serial.print(step_R);
  computeRPM();
  myPID.Compute();
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
  //delay(10);
  }

void rightTurn(double degree)
{
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
  //delay(10);
}

/*
// calibrate robot to correct angle from the wall of front using front right (PS1) and front left (PS3) sensors
// if the angle is correct, both the readings shouldn't deviate much
void CaliAngle()
{
  double targetDist = 10.5;
  int SPEEDL = 70;
  int SPEEDR = 70;
  int count = 0;
  while (getDistance(3) < 30 && getDistance(1) < 30)
  {
    if (count == 150) {
      md.setBrakes(100, 100);
      caliDist();
      break;
    }
    double distDiff = getDistance(3) - getDistance(1);
    if (distDiff >= 0.15 || distDiff <= -0.15) 
    {
      if ((distDiff >= 0.15  && distDiff < 7) && (getDistance(3) >= targetDist)) {
        md.setSpeeds(SPEEDL, 0);
        count++;
      }
      else if ((distDiff <= -0.15 && distDiff > -7) && (getDistance(3) < targetDist)) {
        md.setSpeeds(-SPEEDL, 0);
        count++;
      }
      else if ((distDiff >= 0.15 && distDiff < 7) && (getDistance(3) < targetDist)) {
        md.setSpeeds(0, -SPEEDR);
        count++;
      }
      else if ((distDiff <= -0.15 && distDiff > -7) && (getDistance(3) >= targetDist)) {
        md.setSpeeds(0, SPEEDR);
        count++;
      }
    }
    else
    {
      md.setBrakes(100, 100);
      caliDist();
      break;
    }
  }
}
*/

// calibrate robot to correct angle from the wall of front using front right (PS1) and front left (PS3) sensors
// if the angle is correct, both the readings shouldn't deviate much
double CaliAngle()
{
  double distDiff;
  double distDiff_retVal = 0;
  int count = 0;
  int calSPEED = 50;
  
  while (getDistance(3) < 30 && getDistance(1) < 30)
  {
    // so won't have infinite loop as it will never be fully perfect
    if (count == 15+0) {
      rightTurn(10);
      count = 0;
    }

    // if different is a lot, means the robot is not perpenticular to the wall
    distDiff = getDistance(3) - getDistance(1);

    // assign the original off value to be used for dynamic initial calibration
    if(distDiff > distDiff_retVal)
      distDiff_retVal = distDiff;

    // too tilted to the left, turn to the right
    if (distDiff >= 0.10) 
      md.setSpeeds(-calSPEED, calSPEED);
    // too titled to the right, turn to the left
    else if (distDiff <= -0.0001)
      md.setSpeeds(calSPEED, -calSPEED);
    // correct angle
    else
    {
      caliDist();
      break;
    }
  }

  return distDiff_retVal;
}

// Calibrate robot to targetDist from the wall of front using front right (PS1) and front left (PS3) sensors.
// Uses PS1 and PS3 cause there if those wall/obstacle exist for PS1 and PS3 to calibrate angle, 
//    it can too for distance calibration which there maybe be no wall/obstacle at PS2's direction
void caliDist()
{
  int SPEEDL = 100;
  int SPEEDR = 100;
  int count = 0;

  // infinite WHILE loop till it's completed
  while (1)
  {
    if(count == 1000)
      break;
    
    // if too far off, move closer
    if (getDistance(3) > 5.1 || getDistance(1) > 5.1)
      md.setSpeeds(SPEEDR, SPEEDL);
    // if too close, reverse
    else if (getDistance(3) < 4.4 || getDistance(1) < 4.4)
      md.setSpeeds(-SPEEDR, -SPEEDL);
    // within acceptable range, stop calibrating
    else 
    {
      md.setBrakes(100, 100);
      break;
    }

    ++count;
  }
  md.setBrakes(100, 100);
}

void alignment()
{
  delay(1500);
  for (uint8_t i=0; i<2; i++)
  {
    rightTurn(180);
    delay(100);
    CaliAngle();
    rightTurn(90);
    delay(100);
    CaliAngle();
    rightTurn(90);
    delay(200);
  }
}


//////////////////////////////////////////////////// Obstacle avoidance checklist ////////////////////////////////////////////////////

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
