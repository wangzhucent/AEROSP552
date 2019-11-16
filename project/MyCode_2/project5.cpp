#include "project4.h"
#include "da.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{


	int desiredThrust=0;


	struct timeval start_time, cur_time;
	float nowtime;
	BYTE result;
	SensorReadings_t SensorReadings;
	DSCSAMPLE *samples;
	DSCB dscb;
	DSCADSCAN dscadscan;
	AthenaInit(&samples, &dscb, &dscadscan);
	gettimeofday(&start_time, NULL);

	//Data_Class
	Data_Class Gyro(10);
	Data_Class Mag1(10);
	Data_Class Mag2(10);
	Data_Class Mag3(10);
	Data_Class CSS1(10);
	Data_Class CSS2(10);
	Data_Class CSS3(10);
	Data_Class CSS4(10);
	
	// File
	ofstream file;
	ofstream file2;
	cout<<"Please enter the file name:"<<endl;
	string FileName;
	cin>>FileName;
	file.open(FileName.c_str());

	string FileName2=FileName+"_smooth";
	file2.open(FileName2.c_str());

	// time
	float TotalTime;
	cout<<"Please enter the total time:"<<endl;
	cin>>TotalTime;

	cout<<" Please input the desired thrust: "<<endl;
	cin>>desiredThrust;

	// **** Initialization of timer
	timer_t             timerid;       // timer ID for timer
    struct sigevent     event;       // event to deliver
    struct itimerspec   timer;      // the timer data structure
	int chan_id, connect_id;  // Connection from timer back to this program
	struct _pulse   pulse;    // a pulse -- the "message" sent to unblock this thread
	int i;  // loop counter...

    if ((chan_id = ChannelCreate (0)) == -1) { // Open channel for timer
        printf("Error creating channel.\n");
        exit (-1);
    }

    if ((connect_id = ConnectAttach (0, 0, chan_id, 0, 0)) == -1) {     // Create a connection back to ourselves
        printf ("Error connecting the channel.\n");
        exit (-1);
    }

    // Set up the kind of event to deliver -- a pulse (a standard type)
    SIGEV_PULSE_INIT (&event, connect_id, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

    // Create the timer, binding it to the pulse event
    if (timer_create (CLOCK_REALTIME, &event, &timerid) == -1) {
        printf ("Error creating timer.\n");
        exit (-1);
    }

    // Set up the timer
    timer.it_value.tv_sec = 1;          // initial delay in seconds
    timer.it_value.tv_nsec = 0;        // initial delay nanoseconds (use to store fraction of a second)
    timer.it_interval.tv_sec = 0;       // interval between pulses (seconds)
    timer.it_interval.tv_nsec = 1000000000/100;     // interval between pulses (nanoseconds) (5E8 = 0.5 seconds)
    // ***
    


	/* Start initial A/D Scan */
	if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
	{
		fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
		free(samples);
		return -1;
	}
	printf("Starting DAQ loop.\n");	


	// Start the timer
    timer_settime (timerid, 0, &timer, NULL);

	while (nowtime<TotalTime/2)
	{	
		// dscSleep(100); // Sleep for 100 msec
		// dscSleep(10);	// 10 msec~~100Hz
		MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

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

		Gyro.push(SensorReadings.GyroVolts);
		Mag1.push(SensorReadings.MagXVolts);
		Mag2.push(SensorReadings.MagYVolts);
		Mag3.push(SensorReadings.MagZVolts);
		CSS1.push(SensorReadings.CSS1Volts);
		CSS2.push(SensorReadings.CSS2Volts);
		CSS3.push(SensorReadings.CSS3Volts);
		CSS4.push(SensorReadings.CSS4Volts);

		file2<<nowtime<<'\t'
			<<Gyro.mean()<<'\t'
			<<Mag1.mean()<<'\t'
			<<Mag2.mean()<<'\t'
			<<Mag3.mean()<<'\t'
			<<CSS1.mean()<<'\t'
			<<CSS2.mean()<<'\t'
			<<CSS3.mean()<<'\t'
			<<CSS4.mean()<<endl;

		/* Print results to screen */
		printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);		
	}

	//fan_speed
	
	unsigned short output_code;
	if (desiredThrust >= 0.0) {
      da_write(1, 0);  // Zero negative motor
      output_code = (unsigned short) (4095*(desiredThrust/10.0));
      da_write(3, output_code);
    } else { 
      da_write(3, 0);  // Zero positive motor
      output_code = (unsigned short) (4095*(-desiredThrust/10.0));
      da_write(1, output_code);
    }


    // Start the timer
    timer_settime (timerid, 0, &timer, NULL);
	while (nowtime>=TotalTime/2 && nowtime<TotalTime)
	{	
		// dscSleep(100); // Sleep for 100 msec
		// dscSleep(10);	// 10 msec~~100Hz
		MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

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

		Gyro.push(SensorReadings.GyroVolts);
		Mag1.push(SensorReadings.MagXVolts);
		Mag2.push(SensorReadings.MagYVolts);
		Mag3.push(SensorReadings.MagZVolts);
		CSS1.push(SensorReadings.CSS1Volts);
		CSS2.push(SensorReadings.CSS2Volts);
		CSS3.push(SensorReadings.CSS3Volts);
		CSS4.push(SensorReadings.CSS4Volts);

		file2<<nowtime<<'\t'
			<<Gyro.mean()<<'\t'
			<<Mag1.mean()<<'\t'
			<<Mag2.mean()<<'\t'
			<<Mag3.mean()<<'\t'
			<<CSS1.mean()<<'\t'
			<<CSS2.mean()<<'\t'
			<<CSS3.mean()<<'\t'
			<<CSS4.mean()<<endl;

		/* Print results to screen */
		printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);		
	}
	
	desiredThrust=0;
	if (desiredThrust >= 0.0) {
      da_write(1, 0);  // Zero negative motor
      output_code = (unsigned short) (4095*(desiredThrust/10.0));
      da_write(3, output_code);
    } else { 
      da_write(3, 0);  // Zero positive motor
      output_code = (unsigned short) (4095*(-desiredThrust/10.0));
      da_write(1, output_code);
    }

	dscFree();
	file.close();
	file2.close();
	return 0;
}


