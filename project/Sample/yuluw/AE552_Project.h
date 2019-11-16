#include "read_gyro.h"
#include <stdio.h>
#include "da.h"
#include <dscud.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>  // Defines pulse data structure (QNX)
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

// Data Acquisition Class
// Used to store th most recent sensor readings
class dataAQ {
    vector<double> data;
    int windowSize;
    
public:
    dataAQ(int n) {
        windowSize = n;
    }
    
    void dataPush(double num) {
        if (data.size() < windowSize) {
            data.push_back(num);
        }
        else {
            data.erase(data.begin());
            data.push_back(num);
        }
        
    }
    
    double MA(){
        double average;
        average = accumulate(data.begin(), data.end(), 0.0) / data.size();
        return average;
    }
    
};

// QNX system class
class qnxSystem {
    int MA;             // flag of using moving average
    int freq;           // Target frequency
    double TargetVolts; // Target voltage
    double desiredTime; // Target time
    double P, I, D;     // contorl gains
    double bestAngle;   // store the most recent estimation of heading angle
    vector<vector<double> > pcss; // calibration data for CSS
    vector<vector<double> > pmag; // calibration data for CSS
    
    
    unsigned short output_code;
    struct timeval start_time, cur_time;
    BYTE result;
    SensorReadings_t SensorReadings;
    DSCSAMPLE *samples;
    DSCB dscb;
    DSCADSCAN dscadscan;
    
    timer_t             timerid;       // timer ID for timer
    struct sigevent     event;         // event to deliver
    struct itimerspec   timer;         // the timer data structure
    int chan_id, connect_id;  // Connection from timer back to this program
    struct _pulse   pulse;    // a pulse -- the "message" sent to unblock this thread
    
    
public:
    // Constructor
    qnxSystem(double Volt, double Time, int frequency,\
              int ma, double p, double i,\
              double d) {
        TargetVolts = Volt;
        desiredTime = Time;
        freq = frequency;
        MA = ma;
        P= p;
        I = i;
        D = d;
        bestAngle = -1;
        
        AthenaInit(&samples, &dscb, &dscadscan);
        
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
        
    }
    
    int daq(string);
    int cali(string);
    double getAngleCss(double , double , double , double );
    int point(double, int, int, string);
    double getAngleMag(double, double);
    double getFT(vector<double> , double);
};

