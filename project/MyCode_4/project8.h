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
#include "da.h"
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>


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

class Data_Class
{
	vector<double> data;
	int maxsize;
public:
	Data_Class(int num):maxsize(num)
	{
	}
	void push(double num)
	{
		if (data.size()<maxsize)
			data.push_back(num);
		else
		{
			data.erase(data.begin());
			data.push_back(num);
		}
	}
	double mean()
	{
		double sum=0.0;
		for (int i=0;i<data.size();i++)
			sum+=data[i];
		return sum/data.size();
	}
};

class System_Class
{
	int freq;
	double desiredVal;
	double desiredTime;
	double P, I, D;

	// Initialization of Command_Fan part
	unsigned short output_code;

	// Initialization of Reading Part
	struct timeval start_time, cur_time;
	float nowtime;
	BYTE result;
	SensorReadings_t SensorReadings;
	DSCSAMPLE *samples;
	DSCB dscb;
	DSCADSCAN dscadscan;
	// AthenaInit(&samples, &dscb, &dscadscan);
	// gettimeofday(&start_time, NULL);

	// **** Initialization of timer
	timer_t             timerid;       // timer ID for timer
    struct sigevent     event;       // event to deliver
    struct itimerspec   timer;      // the timer data structure
	int chan_id, connect_id;  // Connection from timer back to this program
	struct _pulse   pulse;    // a pulse -- the "message" sent to unblock this thread
	int i;  // loop counter...
	
	int window_size;
	bool isMean;

	

public:
	System_Class(int freq_in, double desiredVal_in, double desiredTime_in, double P_in, double I_in, double D_in, int window_size_in, bool isMean_in):freq(freq_in), desiredVal(desiredVal_in), desiredTime(desiredTime_in), P(P_in), I(I_in), D(D_in), i(0), window_size(window_size_in), isMean(isMean_in)
	{
		AthenaInit(&samples, &dscb, &dscadscan);
		gettimeofday(&start_time, NULL);

		//check the timer
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
	    timer.it_interval.tv_nsec = 1000000000/freq;     // interval between pulses (nanoseconds) (5E8 = 0.5 seconds)
	    // ***
	}

	int daq(string FileName)
	{
		double error=0;
		double desiredThrust=0;
		ofstream file;
		file.open(FileName.c_str());
		string FileName2=FileName+"_smooth";
		ofstream file2;
		file2.open(FileName2.c_str());

		//Data_Class
		Data_Class Gyro(window_size);
		Data_Class Mag1(window_size);
		Data_Class Mag2(window_size);
		Data_Class Mag3(window_size);
		Data_Class CSS1(window_size);
		Data_Class CSS2(window_size);
		Data_Class CSS3(window_size);
		Data_Class CSS4(window_size);

		// Start the timer
	    timer_settime (timerid, 0, &timer, NULL);

		while (nowtime<desiredTime)
		{	
			nowtime=(float)i/ (float)freq;
			i++;

			MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

			if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
			{
				fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
				return -1;
			}		
			
			gettimeofday(&cur_time, NULL);
			// nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
	  //    		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

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
				<<SensorReadings.CSS4Volts<<'\t';

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
				<<CSS4.mean()<<'\t';

			/* Print results to screen */
			printf("t = %.2f, gyro (V) = %.3lf\n", nowtime, SensorReadings.GyroVolts);
			
			if (isMean)
				error = Gyro.mean()-desiredVal;
			else
				error = SensorReadings.GyroVolts-desiredVal;

			if (abs(error)>0.02)
				desiredThrust=(error/abs(error)*8+P*error);
			else
				desiredThrust=0;
			if (abs(desiredThrust)>10)
				desiredThrust=desiredThrust/abs(desiredThrust)*10;
			else if(abs(desiredThrust)<7)
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


		    file<<desiredThrust<<endl;
		    file2<<desiredThrust<<endl;

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

		while (nowtime<desiredTime+10)
		{	
			nowtime=(float)i/ (float)freq;
			i++;

			MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

			if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
			{
				fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
				return -1;
			}		
			
			gettimeofday(&cur_time, NULL);
			// nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
	  //    		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

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
				<<SensorReadings.CSS4Volts<<'\t'
				<<0<<endl;

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
				<<CSS4.mean()<<'\t'
				<<0<<endl;
		}

		dscFree();
		file.close();
		file2.close();
		return 1;
	}

	int point(double desiredAngle, string FileName, bool isRecord, bool isMonitor)
	{
		double error=0;

		double error_dot=0;
		double error_int=0;

		double desiredThrust=0;
		ofstream file;
		file.open(FileName.c_str());
		string FileName2=FileName+"_smooth";
		ofstream file2;
		file2.open(FileName2.c_str());

		//Data_Class
		Data_Class Gyro(window_size);
		Data_Class Mag1(window_size);
		Data_Class Mag2(window_size);
		Data_Class Mag3(window_size);
		Data_Class CSS1(window_size);
		Data_Class CSS2(window_size);
		Data_Class CSS3(window_size);
		Data_Class CSS4(window_size);

		// Start the timer
	    timer_settime (timerid, 0, &timer, NULL);

		while (nowtime<desiredTime)
		{	
			nowtime=(float)i/ (float)freq;
			i++;

			MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);

			if( (result = dscADScan( dscb, & dscadscan, samples) )!= DE_NONE )
			{
				fprintf(stderr, "%s%s\n", ERROR_PREFIX1, dscGetErrorString(result) );
				return -1;
			}		
			
			gettimeofday(&cur_time, NULL);
			// nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
	  //    		     +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

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
			
			Gyro.push(SensorReadings.GyroVolts);
			Mag1.push(SensorReadings.MagXVolts);
			Mag2.push(SensorReadings.MagYVolts);
			Mag3.push(SensorReadings.MagZVolts);
			CSS1.push(SensorReadings.CSS1Volts);
			CSS2.push(SensorReadings.CSS2Volts);
			CSS3.push(SensorReadings.CSS3Volts);
			CSS4.push(SensorReadings.CSS4Volts);

			



			// The do the transformation: 
			if (isMean)
			{
				double v1=Mag1.mean()-2.5241;
				double v2=Mag2.mean()-2.5585;
				error_dot = -46.9833*Gyro.mean()+112.5519; // Rotation speed deg/sec
				error=atan2((v1+31.5396*v2)/(-2.9082),(v1-0.0605*v2)/0.1121)/3.14159*180-desiredAngle; // deg
				error_int+=error/freq;
			}
			else
			{
				double v1=SensorReadings.MagXVolts-2.5241;
				double v2=SensorReadings.MagYVolts-2.5585;
				error_dot = -46.9833*SensorReadings.GyroVolts+112.5519;
				error=atan2((v1+31.5396*v2)/(-2.9082),(v1-0.0605*v2)/0.1121)/3.14159*180-desiredAngle; // deg
				error_int+=error/freq;
			}

			desiredThrust=P*error+I*error_int+D*error_dot;
			
			desiredThrust=desiredThrust/abs(desiredThrust)*8+desiredThrust; // The thrust cannot work for all range

			// Insurance
			if (abs(desiredThrust)>10)
				desiredThrust=desiredThrust/abs(desiredThrust)*10;
			else if(abs(desiredThrust)<7)
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
  
		    // Record the data in the files
		    if (isRecord)
		    {
			    file<<nowtime<<'\t'
					<<SensorReadings.GyroVolts<<'\t'
					<<SensorReadings.MagXVolts<<'\t'
					<<SensorReadings.MagYVolts<<'\t'
					<<SensorReadings.MagZVolts<<'\t'
					<<SensorReadings.CSS1Volts<<'\t'
					<<SensorReadings.CSS2Volts<<'\t'
					<<SensorReadings.CSS3Volts<<'\t'
					<<SensorReadings.CSS4Volts<<'\t';

				file2<<nowtime<<'\t'
					<<Gyro.mean()<<'\t'
					<<Mag1.mean()<<'\t'
					<<Mag2.mean()<<'\t'
					<<Mag3.mean()<<'\t'
					<<CSS1.mean()<<'\t'
					<<CSS2.mean()<<'\t'
					<<CSS3.mean()<<'\t'
					<<CSS4.mean()<<'\t';

			    file<<desiredThrust<<endl;
			    file2<<desiredThrust<<endl;
			}

			// The the data in real time
			if (isMonitor)
				printf("t = %.2f, \t error (deg) = %.3lf, \t error_dot (deg/sec) = %.3lf, \t thrust (V) = %.3lf\n", nowtime, error, error_dot, desiredThrust);
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
		return 1;
	}
};