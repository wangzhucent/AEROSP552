/* read_gyro.c */

#include "read_gyro.h"

int main()
{
	struct timeval start_time, cur_time;
	float nowtime;
	BYTE result;
	SensorReadings_t SensorReadings;
	DSCSAMPLE *samples;
	DSCB dscb;
	DSCADSCAN dscadscan;
	AthenaInit(&samples, &dscb, &dscadscan);
	gettimeofday(&start_time, NULL);

	/* Start initial A/D Scan */
	if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
	{
		fprintf(stderr, "%s%s\n", ERROR_PREFIX, dscGetErrorString(result) );
		free(samples);
		return -1;
	}
	printf("Starting DAQ loop.\n");	

	while (1)
	{	
		dscSleep(100); // Sleep for 100 msec

		if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
		{
			fprintf(stderr, "%s%s\n", ERROR_PREFIX, dscGetErrorString(result) );
			return -1;
		}		
		
		gettimeofday(&cur_time, NULL);
		nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
     		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

		SensorReadings.TimeStamp = nowtime;
		SensorReadings.GyroVolts =  
		  (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
		
		/* Print results to screen */
		printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);		
	}

	return 0;
}
	
