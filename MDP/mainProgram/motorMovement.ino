volatile double rpm_L; //rpm of left motor
volatile double rpm_R; //rpm of right motor
volatile double step_L; //number of steps taken by the left motor
volatile double step_R; //number of steps taken by the right motor
volatile double error; //difference between left and right steps

volatile const int SPEED =  250; //Set the speed of the motor

// PID Parameters
// PID values for obstacle avoidance checklist because interupt not working well due to delay checking the sensor: Kp=8, Ki=0, Kd=0.008
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
  delay(10);
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
  delay(10);
  }

void leftTurn(double degree){
  
  double totalSteps = totalAngularSteps(degree);

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

void rightTurn(double degree){
  double totalSteps = totalAngularSteps(degree);

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


// calibrate robot to correct angle from the wall of front and left of the robot
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
    if (distDiff >= 0.15 || distDiff <= -0.15) {
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


// calibrate robot to targetDist from the wall of front and left of the robot
void caliDist() {
  int SPEEDL = 70;
  int SPEEDR = 70;
  while (getDistance(2) < 30 && getDistance(4) < 30)//30
  {
    if ((getDistance(2) >= 10.65 && getDistance(2) < 11) || (getDistance(4) >= 10.65 && getDistance(4) < 11))//10.6,11.5 //10.95,11.05
    {
      md.setBrakes(100, 100);
      break;
    }
    else if (getDistance(2) < 10.65 || getDistance(4) < 10.65)
    {
      md.setSpeeds(-SPEEDL, -SPEEDR);
    }
    else {
      md.setSpeeds(SPEEDL, SPEEDR);
    }
  }
  md.setBrakes(100, 100);
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
