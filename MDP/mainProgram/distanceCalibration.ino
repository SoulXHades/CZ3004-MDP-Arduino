const double one_grid = 9.165 *1;
const double two_grid = 9.365 *2;
const double three_grid = 9.365 *3;
const double four_grid = 9.455 *4;
const double five_grid = 9.515 *5;
const double six_grid = 9.515 *6;
const double seven_grid = 9.515 *7;
const double eight_grid = 9.515 *8;
const double nine_grid = 9.515 *9;
const double ten_grid = 9.365 *10;
const double eleven_grid = 9.365 *11;
const double twelve_grid = 9.365 *12;
const double thirteen_grid = 9.365 *13;
const double fourteen_grid = 9.365 *14;
const double fifteen_grid = 9.565 *15;

const double degree_90 = 90; //90 degree turn 86.5
double r_angularOffset = 0;
double l_angularOffset = 0;

double totalRegularSteps(double steps){ //Calculates the number of steps needed to move in a straight line motion
  return ceil(steps * 31.6);
}

double totalAngularSteps(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.38); //4.53
}

double totalAngularSteps_L(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.38); //4.38 (2nd battery - Fully charged)
}

double totalAngularSteps_R(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.375); //4.375 (2nd battery - Fully charged)
}

// to dynamically calibrate the turning degree to counter voltage of battery constant changes at the start of each run
int dynamicAngularCalibration()
{
  double distDiff;

  // align robot properly before doing dynamic calibration
  leftTurn(90);
  delay(100);
  CaliAngle();
  leftTurn(90);
  delay(100);
  CaliAngle();

  // dynamically calibrate left turn
  for (uint16_t i=0; i<20; i++)
  {
    leftTurn(90);
    leftTurn(90);
    leftTurn(90);
    leftTurn(90);
    distDiff = CaliAngle();

    // not turning left enough so increase offset
    if (distDiff >= 0.10)
      l_angularOffset += 0.05;
    // turned left too much so decrease offset
    else if (distDiff <= -0.0001)
      l_angularOffset -= 0.05;
    // roughly good to go hence exit calibrating left turn
    else
      break;
  }

  // dynamically calibrate right turn
  for (uint16_t i=0; i<20; i++)
  {
    rightTurn(90);
    rightTurn(90);
    rightTurn(90);
    rightTurn(90);
    distDiff = CaliAngle();

    // not turning left enough so increase offset
    if (distDiff >= 0.10)
      r_angularOffset += 0.05;
    // turned left too much so decrease offset
    else if (distDiff <= -0.0001)
      r_angularOffset -= 0.05;
    // roughly good to go hence exit calibrating left turn
    else
      break;
  }

  // left robot face the front of the start point
  leftTurn(180);

  // align the robot nicely at the start position
  alignment();
}


double getFinalSteps(int numOfSteps) {
  
    switch(numOfSteps) {
      case 1 : return one_grid;  
                break;
      case 2 : return two_grid; 
                break;
      case 3 : return three_grid; 
                break;
      case 4 : return four_grid;
                break;
      case 5 : return five_grid;  
                break;
      case 6 : return six_grid; 
                break;
      case 7 : return seven_grid; 
                break;
      case 8 : return eight_grid; 
                break;
      case 9 : return nine_grid; 
                break;
      case 10 : return ten_grid;  
                break;
      case 11 : return eleven_grid;  
                break;
      case 12 : return twelve_grid;  
                break;
      case 13 : return thirteen_grid;  
                break;
      case 14 : return fourteen_grid;  
                break;
      case 15 : return fifteen_grid;  
                break;
            
    }
}
