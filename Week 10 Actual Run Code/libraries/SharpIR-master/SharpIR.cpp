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
//    > 1080+ is the int for the GP2Y0A21Y and 
//    > 20150 is the int for GP2Y0A02YK and 
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
			// for 10cm
			if (calibratedDist < 24.0)
				return calibratedDist - 11.0;
			// for 20cm
			else if (calibratedDist < 30.0)
				return calibratedDist - 7.0;
			// for 30cm
			else if (calibratedDist < 40.0)
				return calibratedDist - 7.0;
			// for 40cm
			else if (calibratedDist < 52.0)
				return calibratedDist - 9.0;
			// for 50cm
			else if (calibratedDist < 63.0)
				return calibratedDist - 11.0;
			// for 60cm
			else if (calibratedDist < 76.0)
				return calibratedDist - 14.0;
			// for 70cm
			else if (calibratedDist < 88.0)
				return calibratedDist - 15.0;

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


