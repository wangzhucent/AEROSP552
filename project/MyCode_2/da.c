// D/A motor driver for TableSat
// ch1 = -, ch3 = + motor
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

#include <dscud.h>
#include "da.h"

#define ERROR_PREFIX "PROM Driver ERROR:"

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
		fprintf( stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring );
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
		fprintf(stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring);
		return 0;
	}
}

// Write motor commands (-=ch1, +=ch3 for TableSat)
int da_write(unsigned short channel, unsigned short output_code)
{
 	BYTE result; // returned error code
	ERRPARAMS errorParams; 
	if( ( result = dscDAConvert( dscb, channel, output_code ) ) != DE_NONE ) {
			dscGetLastError(&errorParams);
			fprintf(stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring);
			return -1;
	}
	return 0;
} 
