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
#define ERROR_PREFIX1 "Athena Error: "

#define ERROR_PREFIX2 "PROM Driver ERROR:"

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


void AthenaInit(DSCSAMPLE **samples, DSCB *dscb, DSCADSCAN *dscadscan)
{
	DSCCB 		dsccb; // structure containing board settings
	DSCADSETTINGS 	dscadsettings; // structure containing A/D conversion settings	
	BYTE result;
	ERRPARAMS errorParams;

	/***** Initialize DSCUD library *****/

	if( dscInit( DSC_VERSION ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "%s %s\n", ERROR_PREFIX1, errorParams.errstring );
		return;
	}

	/***** Initialize Prometheus board *****/

	dsccb.boardtype = DSC_ATHENA;
	dsccb.base_address = BASEADD;
	dsccb.int_level = 5;
	dsccb.dma_level = 3;
	dsccb.clock_freq = 10000000L;

	if(dscInitBoard(DSC_ATHENA, &dsccb, dscb)!= DE_NONE)
	{
		dscGetLastError(&errorParams);
		fprintf(stderr, "%s %s\n", ERROR_PREFIX1, errorParams.errstring);
		return;
	}

	/***** Initialize AD settings *****/

	dscadsettings.range = RANGE_5;
	dscadsettings.polarity = UNIPOLAR;
	dscadsettings.gain = GAIN_4;  // Was GAIN_4
	dscadsettings.load_cal = (BYTE)TRUE;
	dscadsettings.current_channel = 0;

	dscadsettings.load_cal = 0;

	if( ( result = dscADSetSettings( *dscb, &dscadsettings ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		fprintf(stderr, "%s %s\n", ERROR_PREFIX1, errorParams.errstring);
		return;

	}

	/***** Set D/A polarity *****/
	dscDASetPolarity(*dscb, UNIPOLAR);

	/***** Initialize scan settings *****/
	dscadscan->low_channel 	= 0;
	dscadscan->high_channel 	= 7;
	// dscadscan->gain		= GAIN_1;
	dscadscan->gain		= dscadsettings.gain;

	*samples = (DSCSAMPLE*) malloc(sizeof(DSCSAMPLE)*(dscadscan->high_channel - dscadscan->low_channel+1));

	return;
}








// Variables used in init and write
DSCB dscb; // handle used to refer to the board

// Initialize D/A on Prometheus board
int da_init(void)
{
 	DSCCB dsccb; // structure containing board settings
 	BYTE result; // returned error code
	ERRPARAMS errorParams; // spits out errors
	
	// Initialize DSCUD driver
	if( dscInit( DSC_VERSION ) != DE_NONE ) { 
		dscGetLastError(&errorParams);
		fprintf( stderr, "%s %s\n", ERROR_PREFIX2, errorParams.errstring );
		return 0;
	}

	// Initialize Prometheus board
	dsccb.boardtype = DSC_ATHENA;
	dsccb.base_address = 0x280;
	dsccb.int_level = 5;
	dsccb.dma_level = 3;
	dsccb.clock_freq = 10000000L;
	if(dscInitBoard(DSC_ATHENA, &dsccb, &dscb)!= DE_NONE) {
		dscGetLastError(&errorParams);
		fprintf(stderr, "%s %s\n", ERROR_PREFIX2, errorParams.errstring);
		return 0;
	}
}

// Write motor commands (-=ch1, +=ch3 for TableSat)
int da_write(unsigned short channel, unsigned short output_code)
{
 	BYTE result; // returned error code
	ERRPARAMS errorParams; 
	if( ( result = dscDAConvert( dscb, channel, output_code ) ) != DE_NONE )
 {
			dscGetLastError(&errorParams);
			fprintf(stderr, "%s %s\n", ERROR_PREFIX2, errorParams.errstring);
			return -1;
	}
	return 0;
} //da.c



int fan_speed(double desiredThrust) 
{
  unsigned short output_code;
  // double xblahblah;
  // double desiredThrust=0.0;

  // Initialize Prometheus hardware

  if (abs(desiredThrust)>10)
  	desiredThrust=desiredThrust/abs(desiredThrust)*10;
  
  if (da_init()) return(-1);
    
    // Set thrust levels
    if (desiredThrust >= 0.0) {
      da_write(1, 0);  // Zero negative motor
      output_code = (unsigned short) (4095*(desiredThrust/10.0));
      da_write(3, output_code);
    } else { 
      da_write(3, 0);  // Zero positive motor
      output_code = (unsigned short) (4095*(-desiredThrust/10.0));
      da_write(1, output_code);
    }
  
  return 1;
}