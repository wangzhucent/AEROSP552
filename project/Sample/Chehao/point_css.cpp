//
//
// tsat1 (Athena) version
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#include "read_gyro.h"
#include <stdio.h>
#include "da.h"
#include <dscud.h>

#include <time.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>  // Defines pulse data structure (QNX)

using namespace std;

double Fourier_7(double x, double a0, double a1, double b1, double a2, double b2, double a3, double b3, double a4, double b4, double a5, double b5, double a6, double b6, double a7, double b7, double w) {
	return a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w) + a4*cos(4*x*w) + b4*sin(4*x*w) + a5*cos(5*x*w) + b5*sin(5*x*w) + a6*cos(6*x*w) + b6*sin(6*x*w)+ a7*cos(7*x*w) + b7*sin(7*x*w);
}

double Fourier_6(double x, double a0, double a1, double b1, double a2, double b2, double a3, double b3, double a4, double b4, double a5, double b5, double a6, double b6, double w) {
	return a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w) + a4*cos(4*x*w) + b4*sin(4*x*w) + a5*cos(5*x*w) + b5*sin(5*x*w) + a6*cos(6*x*w) + b6*sin(6*x*w);
}

double Fourier_5(double x, double a0, double a1, double b1, double a2, double b2, double a3, double b3, double a4, double b4, double a5, double b5, double w) {
	return a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w) + a4*cos(4*x*w) + b4*sin(4*x*w) + a5*cos(5*x*w) + b5*sin(5*x*w);
}

double Fourier_3(double x, double a0, double a1, double b1, double a2, double b2, double a3, double b3, double w) {
	return a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w);
}

double calculate_css1 (double x) {
	double a0, a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, w;
	// 6 Fourier
	if (x<=67.9915) {
		a0 = -1.7;
		a1 = -2.374;
		b1 = 4.208;
		a2 = 3.494;
		b2 = 3.076;
		a3 = 2.518;
		b3 = -2.218;
		a4 = -1.056;
		b4 = -1.44;
		a5 = -0.5518;
		b5 = 0.3476;
		a6 = 0.0646;
		b6 = 0.1135;
		w  = 0.05775;
		return Fourier_6(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,w);
	}
	else if (x>181.0953) {
		a0 =       11.19;
		a1 =       -17.4;
		b1 =       6.765;
		a2 =       9.721;
		b2 =      -9.798;
		a3 =      -3.001;
		b3 =       7.701;
		a4 =     -0.1372;
		b4 =      -3.809;
		a5 =      0.5475;
		b5 =        1.11;
		a6 =     -0.1713;
		b6 =     -0.1642;
		w  =      0.0211;
		return Fourier_6(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,w);
	}
	else 
		return 4.272;
}

double calculate_css2 (double x) {
	double a0, a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, w;
	if (x<=190.8213) {
		a0 =       23.82;
		a1 =       -7.71;
		b1 =      -40.09 ;
		a2 =         -25;
		b2 =        10.2;
		a3 =       7.271;
		b3 =       11.04;
		a4 =       3.046;
		b4 =       -3.07 ;
		a5 =     -0.6423 ;
		b5 =     -0.3959  ;
		w =     0.01681 ;
		return Fourier_5(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,w);
	}
	else if (x>302.9225) {
		a0 =      -1.611;
		a1 =      -5.414;
		b1 =     -0.9307 ;
		a2 =      -4.157 ;
		b2 =        -2.8  ;
		a3 =      -1.637 ;
		b3 =      -3.084 ;
		a4 =     0.07177  ;
		b4 =      -1.811  ;
		a5 =      0.3589  ;
		b5 =     -0.5376  ;
		a6 =      0.1151  ;
		b6 =    -0.04865 ;
		w =     0.06711  ;
		return Fourier_6(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,w);
	}
	else 
		return 4.276;
}

double calculate_css3 (double x) {
	double a0, a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, w;
	if (x<=3.7099) {
		a0 =       154.4  ;
		a1 =      -218.6  ;
		b1 =      -64.32  ;
		a2 =       78.08  ;
		b2 =       51.31  ;
		a3 =      -10.25 ;
		b3 =      -12.16 ;
		w =       0.169 ;
		return Fourier_3(x,a0,a1,b1,a2,b2,a3,b3,w);
	}
	else if (x>115.8111) {
		a0 =       31.84 ;
		a1 =       50.14 ;
		b1 =       25.64 ;
		a2 =       23.96 ;
		b2 =          33 ;
		a3 =       3.513 ;
		b3 =       23.42 ;
		a4 =      -3.373 ;
		b4 =       9.914  ;
		a5 =      -2.284  ;
		b5 =       2.201  ;
		a6 =     -0.4805 ;
		b6 =      0.1404 ;
		w =     0.01465 ;
		return Fourier_6(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,w);
	}
	else
		return 4.28;
}

double calculate_css4 (double x) {
	double a0, a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, a7, b7, w;
	if(x<261.6985) {
		a0 = 0.813171866674789e9;
		a1 = 0;
		b1 = -3.254136566493726e9;
		a2 = -2.516871978755295e9;
		b2 = 3.182262685467437e9;
		a3 = 2.844502720470111e9;
		b3 = -1.100718541671489e9;
		a4 = -1.496710560815133e9;
		b4 = -0.187826115937459e9;
		a5 = 0.400164395757159e9;
		b5 = 0.275534395065641e9;
		a6 = -0.044521432064342e9;
		b6 = -0.081927992753838e9;
		a7 = 0.000264992868549e9;
		b7 = 0.008138156249985e9;
		w =  0.000000000003396e9;
		return Fourier_7(x,a0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,w);
	}
	else
		return 4.292;
}

double calibration_css(double css1_data, double css2_data, double css3_data, double css4_data){
	
	if (css1_data>4.272)
		css1_data=4.272;
	if (css2_data>4.276)
		css2_data=4.276;
	if (css3_data>4.28)
		css3_data=4.28;
	if (css4_data>4.292)
		css4_data=4.292;

	double tol=0.01;
	double degree=0;
	double err0=pow(calculate_css1(0)-css1_data,2)+pow(calculate_css2(0)-css2_data,2)+pow(calculate_css3(0)-css3_data,2)+pow(calculate_css4(0)-css4_data,2);
	double err1=0;
	double pointing=0;
	double step=0.1;
	while (degree<360-step) {
		degree = degree+step;
		err1 = pow(calculate_css1(degree)-css1_data,2)+pow(calculate_css2(degree)-css2_data,2)+pow(calculate_css3(degree)-css3_data,2)+pow(calculate_css4(degree)-css4_data,2);
		if (err1<err0) {
			err0 = err1;
			pointing = degree;
		}
	}
	return pointing;
}

int main () {


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
    timer.it_interval.tv_nsec = 10000000;     // interval between pulses (nanoseconds) (1E7 = 0.01 seconds)


// controller
	double offset = 8.6; // Please slightly increase the offset (to 8.8-9.4) when the battery is low and Tsat doesn't rotate under offset = 8.6
	double y0, y1, e, u;
	double theta_tol = 2.0;
	int ny0=1;

	bool reachy0=false;
	double ty0=0;
	double ty0_start=0;

	// int ntimer=0;

	cout << ("Enter desired pointing direction (in degrees 0 - 359.999)")<<endl;
	cin >> y0;

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

	SensorReadings.TimeStamp = nowtime;
	SensorReadings.GyroVolts = (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
	SensorReadings.CSS1Volts= (double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
	SensorReadings.CSS2Volts= (double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
	SensorReadings.CSS3Volts= (double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
	SensorReadings.CSS4Volts= (double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);

	y1 = calibration_css(SensorReadings.CSS1Volts, SensorReadings.CSS2Volts, SensorReadings.CSS3Volts, SensorReadings.CSS4Volts);
	e=y0-y1;

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
		SensorReadings.GyroVolts = (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS1Volts= (double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS2Volts= (double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS3Volts= (double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
		SensorReadings.CSS4Volts= (double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);

		y1 = calibration_css(SensorReadings.CSS1Volts, SensorReadings.CSS2Volts, SensorReadings.CSS3Volts, SensorReadings.CSS4Volts);
		e=y0-y1;


		if (e>0)
			u= -0.004*e-offset;
		else if (e<0)
			u= -0.004*e+offset;
		else 
			u=0;

		if (SensorReadings.GyroVolts >= 2.45){
			if(u < 0){
				u = u-(-1.8*SensorReadings.GyroVolts+4.41)+17;			
			}
			else if(u > 0){
				u = u+(-1.8*SensorReadings.GyroVolts+4.41);			
			}
		}

		else if (SensorReadings.GyroVolts <= 2.25){
			if(u < 0){
				u = u-(1.8*SensorReadings.GyroVolts-4.05);			
			}
			else if(u > 0){
				u = u+(1.8*SensorReadings.GyroVolts-4.05)-17;			
			}
		}

		if (u > 10){u = 10;}
		else if (u < -10){u = -10;}

		desiredThrust=u;

		if ((abs(e) < theta_tol || abs(e+360) < theta_tol || abs(e-360) < theta_tol) && (SensorReadings.GyroVolts <= 2.45 && SensorReadings.GyroVolts >= 2.25 )){
			da_write(1, 0);  // Zero negative motor
			output_code = (unsigned short) (4095*(0.0/10.0));
			da_write(3, output_code);
			return 0;
		}

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

	da_write(1, 0);  // Zero negative motor
	output_code = (unsigned short) (4095*(0.0/10.0));
	da_write(3, output_code);

	dscFree();

	
  
	return 0;
}

