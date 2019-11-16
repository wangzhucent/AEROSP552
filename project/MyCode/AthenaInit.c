#include "read_data.h"

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
		fprintf( stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring );
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
		fprintf(stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring);
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
		fprintf(stderr, "%s %s\n", ERROR_PREFIX, errorParams.errstring);
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
