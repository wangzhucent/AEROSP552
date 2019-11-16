#include "project1.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

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
	
	// File
	ofstream file;
	cout<<"Please enter the file name:"<<endl;
	string FileName;
	cin>>FileName;
	file.open(FileName.c_str());

	// time
	float TotalTime;
	cout<<"Please enter the total time:"<<endl;
	cin>>TotalTime;

	/* Start initial A/D Scan */
	if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
	{
		fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
		free(samples);
		return -1;
	}
	printf("Starting DAQ loop.\n");	




	while (nowtime<TotalTime/2)
	{	
		// dscSleep(100); // Sleep for 100 msec
		dscSleep(10);	// 10 msec~~100Hz

		if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
		{
			fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
			return -1;
		}		
		
		gettimeofday(&cur_time, NULL);
		nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
     		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

		SensorReadings.TimeStamp = nowtime;
		SensorReadings.GyroVolts =  
		  (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
		
		SensorReadings.MagXVolts=(double) (((short) dscadscan.sample_values[1] + 32768)/65536.0 * 5.0);
		SensorReadings.MagYVolts=(double) (((short) dscadscan.sample_values[2] + 32768)/65536.0 * 5.0);
		SensorReadings.MagZVolts=(double) (((short) dscadscan.sample_values[3] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS1Volts=(double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS2Volts=(double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS3Volts=(double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS4Volts=(double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);
		
		file<<nowtime<<'\t'
			<<SensorReadings.GyroVolts<<'\t'
			<<SensorReadings.MagXVolts<<'\t'
			<<SensorReadings.MagYVolts<<'\t'
			<<SensorReadings.MagZVolts<<'\t'
			<<SensorReadings.CSS1Volts<<'\t'
			<<SensorReadings.CSS2Volts<<'\t'
			<<SensorReadings.CSS3Volts<<'\t'
			<<SensorReadings.CSS4Volts<<endl;

		/* Print results to screen */
		printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);		
	}

	fan_speed(7); //fan_speed
	while (nowtime>=TotalTime/2 && nowtime<TotalTime)
	{	
		// dscSleep(100); // Sleep for 100 msec
		dscSleep(10);	// 10 msec~~100Hz

		if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
		{
			fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
			return -1;
		}		
		
		gettimeofday(&cur_time, NULL);
		nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
     		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

		SensorReadings.TimeStamp = nowtime;
		SensorReadings.GyroVolts =  
		  (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
		
		SensorReadings.MagXVolts=(double) (((short) dscadscan.sample_values[1] + 32768)/65536.0 * 5.0);
		SensorReadings.MagYVolts=(double) (((short) dscadscan.sample_values[2] + 32768)/65536.0 * 5.0);
		SensorReadings.MagZVolts=(double) (((short) dscadscan.sample_values[3] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS1Volts=(double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS2Volts=(double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS3Volts=(double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS4Volts=(double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);
		
		file<<nowtime<<'\t'
			<<SensorReadings.GyroVolts<<'\t'
			<<SensorReadings.MagXVolts<<'\t'
			<<SensorReadings.MagYVolts<<'\t'
			<<SensorReadings.MagZVolts<<'\t'
			<<SensorReadings.CSS1Volts<<'\t'
			<<SensorReadings.CSS2Volts<<'\t'
			<<SensorReadings.CSS3Volts<<'\t'
			<<SensorReadings.CSS4Volts<<endl;

		/* Print results to screen */
		printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);		
	}
	fan_speed(0);
	dscFree();
	file.close();
	return 0;
}


