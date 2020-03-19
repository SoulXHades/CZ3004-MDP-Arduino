/*
	SharpIR

	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK

	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico
    + Remove average and use median
    + Definition of number of sample in .h
    + Define IR pin as input

    Version : 1.1 : Thibaut Mauon
    + Add SHARP GP2Y0A710K0F for 100cm to 500cm by Thibaut Mauron

	https://github.com/guillaume-rico/SharpIR
    
    Original comment from Dr. Marcal Casas-Cartagena :
   The Sahrp IR sensors are cheap but somehow unreliable. I've found that when doing continous readings to a
   fix object, the distance given oscilates quite a bit from time to time. For example I had an object at
   31 cm. The readings from the sensor were mainly steady at the correct distance but eventually the distance
   given dropped down to 25 cm or even 16 cm. That's quite a bit and for some applications it is quite
   unacceptable. I checked the library http://code.google.com/p/gp2y0a21yk-library/ by Jeroen Doggen
   (jeroendoggen@gmail.com) and what the author was doing is to take a bunch of readings and give an average of them

   The present library works similary. It reads a bunch of readings (avg), it checks if the current reading
   differs a lot from the previous one (tolerance) and if it doesn't differ a lot, it takes it into account
   for the mean distance.
   The distance is calculated from a formula extracted from the graphs on the sensors datasheets
   After some tests, I think that a set of 20 to 25 readings is more than enough to get an accurate distance
   Reading 25 times and return a mean distance takes 53 ms. For my application of the sensor is fast enough.
   This library has the formulas to work with the GP2Y0A21Y and the GP2Y0A02YK sensors but exanding it for
   other sensors is easy enough.
*/

#ifdef Arduino
  #include "Arduino.h"
#elif defined(SPARK)
  #include "Particle.h"
  #include "math.h"
#endif
#include "SharpIR.h"

// Initialisation function
//  + irPin : is obviously the pin where the IR sensor is attached
//  + sensorModel is a int to differentiate the two sensor models this library currently supports:
//    > 1080 is the int for the GP2Y0A21Y and 
//    > 20150 is the int for GP2Y0A02YK and 
//    > 100500 is the long for GP2Y0A710K0F
//    The numbers reflect the distance range they are designed for (in cm)
SharpIR::SharpIR(int irPin, long sensorModel) {
  
    _irPin=irPin;
    _model=sensorModel;
    
    // Define pin as Input
    pinMode (_irPin, INPUT);
    
    #ifdef ARDUINO
      analogReference(DEFAULT);
    #endif
}

SharpIR::SharpIR(int irPin, long sensorModel, int analogCalData[], int calDistance[], int numOfDataPoints) {
  
    _irPin=irPin;
    _model=sensorModel;
    _analogCalData = analogCalData;
    _calDistance = calDistance;
    _numOfDataPoints = numOfDataPoints;
    
    // Define pin as Input
    pinMode (_irPin, INPUT);
    
    #ifdef ARDUINO
      analogReference(DEFAULT);
    #endif
}

// Sort an array
void SharpIR::sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        bool flag = true;
        for(int o=0; o<(size-(i+1)); o++) {
            if(a[o] > a[o+1]) {
                int t = a[o];
                a[o] = a[o+1];
                a[o+1] = t;
                flag = false;
            }
        }
        if (flag) break;
    }
}


// Sort an array
void SharpIR::sort(float a[], int size) {
    for(int i=0; i<(size-1); i++) {
        bool flag = true;
        for(int o=0; o<(size-(i+1)); o++) {
            if(a[o] > a[o+1]) {
                int t = a[o];
                a[o] = a[o+1];
                a[o+1] = t;
                flag = false;
            }
        }
        if (flag) break;
    }
}


/*
// Original method most people uses
// Read distance and compute it
int SharpIR::distance() {

    int ir_val[NB_SAMPLE];
    int distanceCM;
    float current;


    for (int i=0; i<NB_SAMPLE; i++){
        // Read analog value
        ir_val[i] = analogRead(_irPin);
    }
    
    // Sort it 
    sort(ir_val,NB_SAMPLE);

    
    if (_model==1080) 
    {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.173);
        #elif defined(SPARK)
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.173);
        #endif

    } 
    else if (_model==1081) 
    {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.173);
        #elif defined(SPARK)
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.173);
        #endif

    } 
    else if (_model==1082) 
    {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.173);
        #elif defined(SPARK)
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.173);
        #endif

    } 
    else if (_model==1083) 
    {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.173);
        #elif defined(SPARK)
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.173);
        #endif

    } 
    else if (_model==1084) 
    {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.173);
        #elif defined(SPARK)
          distanceCM = 29.988 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.173);
        #endif

    } 
    else if (_model==20150)
    {

        // Previous formula used by  Dr. Marcal Casas-Cartagena
        // puntualDistance=61.573*pow(voltFromRaw/1000, -1.1068);
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 60.374 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.16);
        #elif defined(SPARK)
          distanceCM = 60.374 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.16);
        #endif

    }

    return distanceCM;
}
*/

/*
// linear interpolation method
int SharpIR::distance()
{

	int ir_val[NB_SAMPLE];

	for (int i=0; i<NB_SAMPLE; i++){
        // Read analog value
        ir_val[i] = analogRead(_irPin);
    }
    
    // Sort it 
    sort(ir_val,NB_SAMPLE);

    int mediumIR_val = ir_val[NB_SAMPLE/2];

	for(int i = 0; i < _numOfDataPoints - 1; i++)
	{
		if(_analogCalData[i] < mediumIR_val && mediumIR_val < _analogCalData[i+1])
		{
			// https://blogs.ntu.edu.sg/scemdp-201718s1-g14/ir-sensors/
			// linear interpolation: distance = y1 + (x - x1) * ((y2 - y1) / (x2 - x1))
			return _calDistance[i+1] + (mediumIR_val - _analogCalData[i+1]) * ((_calDistance[i] -  _calDistance[i+1]) / (_analogCalData[i] - _analogCalData[i+1]));
		}

	}
}
*/


// method to be used
float SharpIR::distance()
{
	const int numOfSamples = 11;
	int ir_val;
	float calibratedDist;
	float distanceList[numOfSamples]; // to store all IR sensor analog raw value
	float volts;

	// divide model number by 10 to reduce repeated code for each individual short range sensors
	switch(_model/10)
	{
		// the model number that is 108 series (actually 1080+ series) are short range IR sensor
		case 108:
			// source: https://wiki.dfrobot.com/Sharp_GP2Y0A21_Distance_Sensor__10-80cm___SKU_SEN0014_
			for(int i = 0; i < numOfSamples ; i++)
			{
				// get raw analog data from IR sensor
				ir_val = analogRead(_irPin);


				if (ir_val < 10)
					ir_val = 10;

			    distanceList[i] = ((67870.0 / (ir_val - 3.0)) - 40.0)/10;
			}

			break;

		// model number == 2015 (actuall 20150) is for long range IR sensor
		case 2015:
			for(int i = 0; i < numOfSamples ; i++)
			{
				volts = analogRead(_irPin)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
			  	distanceList[i] = 65*pow(volts, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)
			}

			break;
	}

	// sort in ascending order
    sort(distanceList, numOfSamples);


    // offset calibration result for each individual sensors
	switch(_model)
	{
		// PS1 (short range IR sensor)
		case 1080:
			calibratedDist = distanceList[numOfSamples/2];

			
			// offset:
			// for 10cm
			if (calibratedDist < 17.5)
				return calibratedDist - 5.0;
			// for 20cm
			else if (calibratedDist < 30.0)
				return calibratedDist - 7.0;
			// for 30cm
			else if (calibratedDist < 37.5)
				return calibratedDist - 8.0;
			// for 40cm
			else if (calibratedDist < 42.0)
				return calibratedDist;
			// for 50cm
			else if (calibratedDist < 45.0)
				return calibratedDist + 8.0;

			return calibratedDist;

		// best sensor with stable readings still 40cm
		// PS4 (short range Front Center IR sensor)
		case 1083:
			calibratedDist = distanceList[numOfSamples/2];

			// offset:
			// for 10cm
			if (calibratedDist < 15.0)
				return calibratedDist - 2.0;
			// for 20cm
			else if (calibratedDist < 30.0)
				return calibratedDist - 5.0;
			// for 30cm
			else if (calibratedDist < 40.0)
				return calibratedDist - 5.0;
			// for 40cm
			else if (calibratedDist < 47.0)
				return calibratedDist - 5.0;
			// for 50cm (Not accurate as too unstable)
			else if (calibratedDist < 70.0)
				return calibratedDist;

			return calibratedDist;

		// PS3 (short range IR sensor)
		case 1082:
			calibratedDist = distanceList[numOfSamples/2];

			// offset:
			// for 10cm
			if (calibratedDist < 20.0)
				return calibratedDist - 5.0;
			// for 20cm
			else if (calibratedDist < 35.0)
				return calibratedDist - 8.0;
			// for 30cm
			else if (calibratedDist < 45.0)
				return calibratedDist - 11.0;
			// for 40cm (need to test more)
			else if (calibratedDist < 55.0)
				return calibratedDist - 7.0;
			// for 50cm (no longer stable)
			else if (calibratedDist < 75.0)
				return calibratedDist - 17.0;

			// return original detected distance
			return calibratedDist;
			break;

		// PS2 (short range IR sensor)
		case 1081:
			// calibrate distance based on percentage
			calibratedDist = distanceList[numOfSamples/2] * (10.0/13.0);

			// offset:
			// for 10cm
			if (calibratedDist < 15.0)
				return calibratedDist - 3.08;
			// for 20cm
			else if (calibratedDist < 28.0)
				return calibratedDist - 3.85;
			// for 30cm
			else if (calibratedDist < 40)
				return calibratedDist - 6.15;
			// for 40cm
			else if (calibratedDist < 60.0)
				return calibratedDist - 17.69;
			// too unstable so don't want to affect the correct readings
			else
				return calibratedDist + 30.0;

			// return original detected distance
			return calibratedDist;

		// PS5 (short range IR sensor)
		case 1084:
			calibratedDist = distanceList[numOfSamples/2];

			// offset:
			// for 10cm
			if (calibratedDist < 18.5)
				return calibratedDist + 1.0;
			// for 20cm
			else if (calibratedDist < 31.0)
				return calibratedDist - 2.0;
			// for 30cm
			else if (calibratedDist < 43.0)
				return calibratedDist - 5.0;
			// for 40cm
			else if (calibratedDist < 55.0)
				return calibratedDist - 8.0;
			// for 50cm
			else if (calibratedDist < 67.0)
				return calibratedDist - 13.0;

			return calibratedDist;

		// PS6 (long range IR sensor)
		case 20150:
			calibratedDist = distanceList[numOfSamples/2];

			// offset:
			// for 30cm
			if (calibratedDist < 35.0)
				return calibratedDist;
			// for 40cm
			else if (calibratedDist < 45.0)
				return calibratedDist - 1.0;
			// for 50cm
			else if (calibratedDist < 60.0)
				return calibratedDist - 3.0;
			// for 60cm
			else if (calibratedDist < 70.0)
				return calibratedDist - 7.0;
			// for 70cm
			else if (calibratedDist < 85.0)
				return calibratedDist - 7.0;
			// for 80cm
			else if (calibratedDist < 97.0)
				return calibratedDist - 13.0;

			return calibratedDist;
	}
  	
  	// return medium result in cm unit.
  	return distanceList[numOfSamples/2];
}


// to get the medium of IR sensor analog raw value
int SharpIR::getMediumAnalogVal()
{
	int ir_val[31];


    for (int i=0; i<31; i++){
        // Read analog value
        ir_val[i] = analogRead(_irPin);
    }
    
    // sort in ascending order
    sort(ir_val,31);

    // return medium
    return ir_val[15];
}


