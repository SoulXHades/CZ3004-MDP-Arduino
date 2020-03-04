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

double totalRegularSteps(double steps){ //Calculates the number of steps needed to move in a straight line motion
  return ceil(steps * 31.6);
}

double totalAngularSteps(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.38); //4.53
}

double totalAngularSteps_L(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.4); //4.53,.18
}

double totalAngularSteps_R(double degree){ //Calculates the number of steps needed to rotate in a angular motion
  return ceil(degree * 4.38); //4.53
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
