int pinLeft = 13; //Set pin 13 as left motor
int pinRight = 5; //Set pin 5 as right motor

void motorInit(){ //Initialize motor and PID
  md.init(); //Define pinMode for the pins and set the frequency for timer1
  
  pinMode(pinLeft, INPUT); //Initialize pinLeft as input
  pinMode(pinRight, INPUT); //Initialize pinRight as input
  
  enableInterrupt(pinLeft, leftSteps, RISING); //Enable interrupts for both motors
  enableInterrupt(pinRight, rightSteps, RISING); //Invokes step counter functions on the rising edge

  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  //myPID.SetOutputLimits(-400, 400);
  myPID.SetSampleTime(5);
  //Adjust the PID values
  //myPID.SetTunings(Kp, Ki, Kd);
  }

void motorStart(){ //Start motor
  md.setSpeeds(0, 0);
  md.setBrakes(0, 0);

  rpm_L = 0;
  rpm_R = 0;
  step_L = 0;
  step_R = 0;
  error = 0;
  }

void motorStop(){ //Stop Motor
  md.setSpeeds(0, 0); 
  md.setBrakes(400, 400);
  }

void leftSteps(){ //Increments the number of steps taken by the left motor
  step_L++;
  }

void rightSteps(){ //Increments the number of steps taken by the right motor
  step_R++;
  }

void computeRPM(){ //Computes the rpm of both left and right motors
  rpm_L = pulseIn(pinLeft, HIGH);
  rpm_R = pulseIn(pinRight, HIGH);

  rpm_L = 60000/(1.1245*rpm_L);
  rpm_R = 60000/(1.1245*rpm_R);
  }

/*Testing Functions*/
void leftMotorRpm() { //Prints individually the rpm of the left motor
  rpm_L = pulseIn(pinLeft, HIGH);                            
  rpm_L = 60000/(1.1245*rpm_L);
  Serial.print(rpm_L);
  Serial.print("\n");
}

void rightMotorRPM() { //Prints individually the rpm of the right motor
  rpm_R = pulseIn(pinRight, HIGH);                          
  rpm_R = 60000/(1.1245*rpm_R);
  Serial.print(rpm_R);
  Serial.print("\n");
}

/*Step Test -> to be placed in void loop() for testing*/
//speed_300();
//Serial.print("Start of speed 250\n");
//speed_250();
//Serial.print("End of speed 250\n");
//md.setBrakes(400, 400);

/*RPM Test (Both motors) -> to be placed in void loop() for testing*/
//Serial.print("LeftM: ");
//Serial.print(rpm_L);
//Serial.print("  ");
//Serial.print("RightM: ");
//Serial.print(rpm_R);
//Serial.print("\n");

//double durationL = pulseIn(pinLeft, HIGH); //Reads the analog values from left motor
//double durationR = pulseIn(pinRight, HIGH); //Reads the analog values from right motor
