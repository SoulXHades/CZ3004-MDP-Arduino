const double one_grid = 9.29 *1; // let it go slightly more than a grid to counter skidding on the spot
const double two_grid = 9.29   *2;
const double three_grid = 9.38 *3;
const double four_grid = 9.455 *4;
const double five_grid = 9.515 *5;
const double six_grid = 9.515 *6;
const double seven_grid = 9.515 *7;
const double eight_grid = 9.515 *8;
const double nine_grid = 9.555 *9;
const double ten_grid = 9.365 *10;
const double eleven_grid = 9.365 *11;
const double twelve_grid = 9.365 *12;
const double thirteen_grid = 9.365 *13;
const double fourteen_grid = 9.365 *14;
const double fifteen_grid = 9.565 *15;

const double degree_90 = 90; //90 degree turn 86.5

// for dynamic angular offset (deciding if we need dynamic angular calibration or not)
double r_angularOffset = 0.0;
double l_angularOffset = 0.0;

double totalRegularSteps(double steps){ //Calculates the number of steps needed to move in a straight line motion
  return ceil(steps * 31.6);
}

double totalAngularSteps(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.38); //4.53
}

double totalAngularSteps_L(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * (4.29+l_angularOffset)); //4.345               (2nd battery - voltage 6.33) (Weight consist of RPi with 2 of the given power banks)
}

double totalAngularSteps_R(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * (4.243+r_angularOffset)); //4.33 (2nd battery - voltage 6.33) (Weight consist of RPi with 2 of the given power banks)
}

// (deciding if we need dynamic angular calibration or not)
// to dynamically calibrate the turning degree to counter voltage of battery constant changes at the start of each run
int dynamicAngularCalibration()
{
  double distDiff;

  // align robot properly before doing dynamic calibration
  leftTurn(90);
  delay(100);
  CaliAngle(true, true);
  leftTurn(90);
  delay(100);
  CaliAngle(true, false);

  // dynamically calibrate left turn
  for (uint16_t i=0; i<10; i++)
  {
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    distDiff = CaliAngle(true, false);

    // not turning left enough so increase offset
    if (distDiff >= 0.05)
      l_angularOffset -= 0.05;
    // turned left too much so decrease offset
    else if (distDiff <= -0.05)
      l_angularOffset += 0.05;
    // roughly good to go hence exit calibrating left turn
    else
      break;
  }

  // dynamically calibrate left turn
  for (uint16_t i=0; i<20; i++)
  {
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    leftTurn(90);
    delay(100);
    distDiff = CaliAngle(true, false);

    // not turning left enough so increase offset
    if (distDiff >= 0.01)
      l_angularOffset -= 0.01;
    // turned left too much so decrease offset
    else if (distDiff <= -0.01)
      l_angularOffset += 0.01;
    // roughly good to go hence exit calibrating left turn
    else
      break;
  }

  // dynamically calibrate right turn
  for (uint16_t i=0; i<10; i++)
  {
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    distDiff = CaliAngle(true, false);

    // not turning left enough so increase offset
    if (distDiff >= 0.05)
      r_angularOffset += 0.05;
    // turned left too much so decrease offset
    else if (distDiff <= -0.05)
      r_angularOffset -= 0.05;
    // roughly good to go hence exit calibrating left turn
    else
      break;
  }

  // dynamically calibrate right turn
  for (uint16_t i=0; i<20; i++)
  {
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    rightTurn(90);
    delay(100);
    distDiff = CaliAngle(true, false);

    // not turning left enough so increase offset
    if (distDiff >= 0.01)
      r_angularOffset += 0.01;
    // turned left too much so decrease offset
    else if (distDiff <= -0.01)
      r_angularOffset -= 0.01;
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
