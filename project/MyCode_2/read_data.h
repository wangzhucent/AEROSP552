#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>
#include <dscud.h>
#include <stdlib.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <math.h>
#include <string.h>

#define BASEADD 0x280
#define ERROR_PREFIX "Athena Error: "

void AthenaInit(DSCSAMPLE **samples, DSCB *dscb, DSCADSCAN *dscadscan);

typedef struct
{
	float TimeStamp;
	double CSS1Volts;
	double CSS2Volts;
	double CSS3Volts;
	double CSS4Volts;
	double GyroVolts;
	double MagXVolts;
	double MagYVolts;
	double MagZVolts;
} SensorReadings_t;

