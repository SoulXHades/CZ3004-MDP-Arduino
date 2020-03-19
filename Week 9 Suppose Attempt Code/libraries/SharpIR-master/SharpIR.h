/*
	SharpIR

	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK

	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico

	https://github.com/guillaume-rico/SharpIR

*/

#ifndef SharpIR_h
#define SharpIR_h

#define NB_SAMPLE 25

#ifdef ARDUINO
  #include "Arduino.h"
#elif defined(SPARK)
  #include "Particle.h"
#endif

class SharpIR
{
  public:

    SharpIR (int irPin, long sensorModel);
    SharpIR(int irPin, long sensorModel, int analogCalData[], int calDistance[], int numOfDataPoints);
    //int distance();
    float distance();
    int getMediumAnalogVal();

  private:

    void sort(int a[], int size);
    void sort(float a[], int size);
    
    int *_analogCalData;
    int *_calDistance;
    int _irPin;
    long _model;
    int _numOfDataPoints;
};

#endif
