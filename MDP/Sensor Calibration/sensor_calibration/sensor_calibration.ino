#include <RunningMedian.h>
#include <SharpIR.h>

#define PS1 A0 // short
#define PS2 A1 // short
#define PS3 A2 // short
#define PS4 A3 // short
#define PS5 A4 // short
#define PS6 A5 // long

#define SAMPLE_SIZE 21

String testOfTextFromRPi;
String sensorResult;
char buffer[25];


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

void setup() {
  Serial.begin(115200); // start the serial port
}

void loop() {
  // read incoming commands from Raspberry Pi
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    Serial.readBytes(buffer, 25);

    // say what you got:
    //Serial.print("I received: ");
    //Serial.write(buffer);
  }

  //Serial.readBytes(buffer, 25);

  // Example of receiving data from RPi and to RPi
  /*
  if(strcmp(buffer, "Hello\n") == 0)
    Serial.write("Hello RPi!\n");
  else if(strcmp(buffer, "") > 0)
    Serial.write("Not empty string!\n");
  else if(strcmp(buffer, "") == 0)
    Serial.write("Empty string!\n");

  memset(buffer, 0, sizeof buffer);\
  */

  calibrateSensor();

  delay(1000);
}

void calibrateSensor()
{
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

  Serial.print(runningMedian_PS5.getMedian());
  Serial.print("\n");


  // to return obstacle location via grid
  // PS1
  sensorResult += "PS1:";
  if (runningMedian_PS1.getMedian() < 12.0)
    sensorResult += "1,";
  else if (runningMedian_PS1.getMedian() < 24.0)
    sensorResult += "2,";
  else
    sensorResult += "-1,";

  // to return obstacle location via grid
  // PS2
  sensorResult += "PS2:";
  if (runningMedian_PS2.getMedian() < 12.5)
    sensorResult += "1,";
  else if (runningMedian_PS2.getMedian() < 22.0)
    sensorResult += "2,";
  else
    sensorResult += "-1,";

  // to return obstacle location via grid
  // PS3
  sensorResult += "PS3:";
  if (runningMedian_PS3.getMedian() < 12.5)
    sensorResult += "1,";
  else if (runningMedian_PS3.getMedian() < 23.0)
    sensorResult += "2,";
  else
    sensorResult += "-1,";

  // to return obstacle location via grid
  // PS4
  sensorResult += "PS4:";
  if (runningMedian_PS4.getMedian() < 12.5)
    sensorResult += "1,";
  else if (runningMedian_PS4.getMedian() < 22.0)
    sensorResult += "2,";
  else
    sensorResult += "-1,";

  // to return obstacle location via grid
  // PS5
  sensorResult += "PS5:";
  if (runningMedian_PS5.getMedian() < 10.0)
    sensorResult += "1,";
  else if (runningMedian_PS5.getMedian() < 21.0)
    sensorResult += "2,";
  else
    sensorResult += "-1,";

  // to return obstacle location via grid
  // PS6
  sensorResult += "PS6:";
  if (25.0 <= runningMedian_PS6.getMedian() && runningMedian_PS6.getMedian() < 34.0)
    sensorResult += "3";
  else if (runningMedian_PS6.getMedian() < 43.0)
    sensorResult += "4";
  else if (runningMedian_PS6.getMedian() < 55.0)
    sensorResult += "5";
  else if (runningMedian_PS6.getMedian() < 64.0)
    sensorResult += "6";
  else
    sensorResult += "-1";

  Serial.println(sensorResult);
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
