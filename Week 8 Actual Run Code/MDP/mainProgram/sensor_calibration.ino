#define PS1 A0 // short
#define PS2 A1 // short
#define PS3 A2 // short
#define PS4 A3 // short
#define PS5 A4 // short
#define PS6 A5 // long

#define SAMPLE_SIZE 21

SharpIR sharpIR_PS1(PS1, 1080);
SharpIR sharpIR_PS2(PS2, 1081);
SharpIR sharpIR_PS3(PS3, 1082);
SharpIR sharpIR_PS4(PS4, 1083);
SharpIR sharpIR_PS5(PS5, 1084);
SharpIR sharpIR_PS6(PS6, 20150);

RunningMedian runningMedian_PS1(SAMPLE_SIZE);
RunningMedian runningMedian_PS2(SAMPLE_SIZE);
RunningMedian runningMedian_PS3(SAMPLE_SIZE);
RunningMedian runningMedian_PS4(SAMPLE_SIZE);
RunningMedian runningMedian_PS5(SAMPLE_SIZE);
RunningMedian runningMedian_PS6(SAMPLE_SIZE);

//int PS1_value;
 
//int PS3_value;
//int PS4_value;
//int PS5_value;
//int PS6_value;

//void sensorInit(){
//  PS1_value = -1;
//  PS2_value = -1;
//  PS3_value = -1;
//  PS4_value = -1;
//  PS5_value = -1;
//  PS6_value = -1;
//  }


// get distance of robot from the wall/obstacle from that sensor
double getDistance(int sensorNum)
{
  switch(sensorNum)
  {
    // PS1 (Short Front Right)
    case 1:
      return sharpIR_PS1.distance();

    // PS2 (Shoft Left Right)
    case 2:
      return sharpIR_PS2.distance();

    // PS3 (Short Front Left)
    case 3:
      return sharpIR_PS3.distance();

    // PS4 (Short Front Center)
    case 4:
      return sharpIR_PS4.distance();

    // PS5 (Short Left Center)
    case 5:
      return sharpIR_PS5.distance();

    // PS6 (Short Right Center)
    case 6:
      return sharpIR_PS6.distance();
  }
}


void getObstacleLocations()
{
  // clear string
  sensorResult = "";
  
  // clear all data
  runningMedian_PS1.clear();
  runningMedian_PS2.clear();
  runningMedian_PS3.clear();
  runningMedian_PS4.clear();
  runningMedian_PS5.clear();
  runningMedian_PS6.clear();

  // to get reading and add them to the median calculator library
  for(int i = 0; i < SAMPLE_SIZE ; i++)
  {
    runningMedian_PS1.add(sharpIR_PS1.distance());
    runningMedian_PS2.add(sharpIR_PS2.distance());
    runningMedian_PS3.add(sharpIR_PS3.distance());
    runningMedian_PS4.add(sharpIR_PS4.distance());
    runningMedian_PS5.add(sharpIR_PS5.distance());
    runningMedian_PS6.add(sharpIR_PS6.distance());
  }

  Serial.println(runningMedian_PS2.getMedian());  // debug

  // to return obstacle location via grid
  // PS1 (Short Front Right)
  if (runningMedian_PS1.getMedian() < 12.0)
    sensorResult += "1,";
  else if (runningMedian_PS1.getMedian() < 21.0)
    sensorResult += "2,";
  else
    sensorResult += "8,";

  // to return obstacle location via grid
  // PS4 (Short Front Center)
  if (runningMedian_PS4.getMedian() < 11.0)
    sensorResult += "1,";
  else if (runningMedian_PS4.getMedian() < 20.0)
    sensorResult += "2,";
  else
    sensorResult += "8,";

  // to return obstacle location via grid
  // PS3 (Short Front Left)
  if (runningMedian_PS3.getMedian() < 13.0)
    sensorResult += "1,";
  else if (runningMedian_PS3.getMedian() < 24.0)
    sensorResult += "2,";
  else
    sensorResult += "8,";

  // to return obstacle location via grid
  // PS2 (Short Left Right)
  if (runningMedian_PS2.getMedian() < 11.0)
    sensorResult += "1,";
  else if (runningMedian_PS2.getMedian() < 21.0)
    sensorResult += "2,";
  else
    sensorResult += "8,";

  // to return obstacle location via grid
  // PS5 (Short Left Center)
  if (runningMedian_PS5.getMedian() < 11.0)
    sensorResult += "1,";
  else if (runningMedian_PS5.getMedian() < 21.0)
    sensorResult += "2,";
  else
    sensorResult += "8,";

  // to return obstacle location via grid
  // PS6 (Long Right Center)
  if (runningMedian_PS6.getMedian() < 25.0)
    sensorResult += "8\n";
  else if (runningMedian_PS6.getMedian() < 34.0)
    sensorResult += "3\n";
  else if (runningMedian_PS6.getMedian() < 43.0)
    sensorResult += "4\n";
  else if (runningMedian_PS6.getMedian() < 55.0)
    sensorResult += "5\n";
  else if (runningMedian_PS6.getMedian() < 64.0)
    sensorResult += "6\n";
  else
    sensorResult += "8\n";

  //Serial.println(sensorResult); // debug
}

void PS2_obstacleDetection()
{
  // clear all data
  runningMedian_PS2.clear();

  // to get reading and add them to the median calculator library
  for(int i = 0; i < SAMPLE_SIZE ; i++)
  {
    runningMedian_PS2.add(sharpIR_PS2.distance());
  }


  // to return obstacle location via grid
  // PS2
  if (runningMedian_PS2.getMedian() < 12.5)
    PS2_value = 1;
  else if (runningMedian_PS2.getMedian() < 22.0)
    PS2_value = 2;
  else
    PS2_value = 8;
}

/*
// https://wiki.dfrobot.com/Sharp_GP2Y0A21_Distance_Sensor__10-80cm___SKU_SEN0014_
// stop range
void loop () {
    uint16_t value = analogRead (PS5);
    uint16_t range = get_gp2d12 (value);
    Serial.println (value);
    Serial.print (range);
    Serial.println (" mm");
    Serial.println ();
    delay (500);
}

uint16_t get_gp2d12 (uint16_t value) {
    if (value < 10) value = 10;
    return ((67870.0 / (value - 3.0)) - 40.0);
}
*/
