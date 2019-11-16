//
//
// tsat1 (Athena) version
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "read_gyro.h"
#include <stdio.h>
#include "da.h"
#include <dscud.h>

#include <time.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>  // Defines pulse data structure (QNX)

using namespace std;


int main () {

	ofstream datafile;
	datafile.open("tsat_data.txt");

// read_gyro
	struct timeval start_time, cur_time;
	float nowtime;
	BYTE result;
	SensorReadings_t SensorReadings;
	DSCSAMPLE *samples;
	DSCB dscb;
	DSCADSCAN dscadscan;
	AthenaInit(&samples, &dscb, &dscadscan);
	gettimeofday(&start_time, NULL);

// command_fan
	int i;
	unsigned short output_code;
	double xblahblah;
	double desiredThrust=0.0;

// timer
	timer_t             timerid;       // timer ID for timer
    struct sigevent     event;       // event to deliver
    struct itimerspec   timer;      // the timer data structure
	int chan_id, connect_id;  // Connection from timer back to this program
	struct _pulse   pulse;    // a pulse -- the "message" sent to unblock this thread
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
    timer.it_interval.tv_nsec = 5000000;     // interval between pulses (nanoseconds) (5E6 = 0.005 seconds)


// controller
	float y0, y1, e, u;
	int ny0=1;

	bool reachy0=false;
	float ty0=0;
	float ty0_start=0;

	cout<<("Enter desired rotation speed (in gyro-Volt units 0 to 5V)")<<endl;
	cin>>y0;

    // Start the timer after receive input
    timer_settime (timerid, 0, &timer, NULL);

	// read gyro
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
		MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

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

		SensorReadings.MagXVolts= (double) (((short) dscadscan.sample_values[1] + 32768)/65536.0 * 5.0);
		SensorReadings.MagYVolts= (double) (((short) dscadscan.sample_values[2] + 32768)/65536.0 * 5.0);
		SensorReadings.MagZVolts= (double) (((short) dscadscan.sample_values[3] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS1Volts= (double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS2Volts= (double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS3Volts= (double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS4Volts= (double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);

		y1=SensorReadings.GyroVolts;
		e=y0-y1;

		if (e>0)
			u=-0.53*e-8.5;
		else if (e<0)
			u=-0.63*e+8.45;
		else 
			u=0;
		if (u > 10){u = 10;}
		if (u < -10){u = -10;}

		if (e>=-0.01 && e<=0.01 && ny0==1) {
			if (reachy0==false) {
				reachy0=true;
				ty0_start=nowtime;
				ny0++;
			}
			else{
	      			da_write(1, 0);  // Zero negative motor
	      			output_code = (unsigned short) (4095*(0.0/10.0));
	      			da_write(3, output_code);
				return 0;
			}
		}

		ty0=nowtime-ty0_start;
		if (reachy0==true && ty0>=79.99 && ty0<=80.01) {
			y0=2.37;
			ny0=1;
		}

		desiredThrust=u;

		/* Print results to screen */
		printf("t = %.3f, thrust = %.3lf, gyro (V) = %.3lf\n", nowtime, desiredThrust, y1);	

		datafile << nowtime <<" ";
		datafile << y1 <<" ";
		datafile << SensorReadings.MagXVolts <<" ";
		datafile << SensorReadings.MagYVolts <<" ";
		datafile << SensorReadings.MagZVolts <<" ";
		datafile << SensorReadings.CSS1Volts <<" ";
		datafile << SensorReadings.CSS2Volts <<" ";
		datafile << SensorReadings.CSS3Volts <<" ";
		datafile << SensorReadings.CSS4Volts <<" ";
		datafile << endl;

		// Set thrust levels
	    if (desiredThrust >= 0.0) {
	      da_write(1, 0);  // Zero negative motor
	      output_code = (unsigned short) (4095*(desiredThrust/10.0));
	      da_write(3, output_code);
	    } 
	    else { 
	      da_write(3, 0);  // Zero positive motor
	      output_code = (unsigned short) (4095*(-desiredThrust/10.0));
	      da_write(1, output_code);
	    }

	    
	}

	dscFree();
	datafile.close();
  
	return 0;
}

