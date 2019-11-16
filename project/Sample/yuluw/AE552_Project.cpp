#include "AE552_Project.h"
/********************************************************/
// Member Function daq()
// Given target voltage, frequncy, time, and filename
// Maintain the target voltage for target time
// and write data to a txt file with a name of [filename]
/********************************************************/

int qnxSystem::daq(string fileName) {
    
    int start = -1; // target speed counter
    int flag = 1;   // program return flag
    double i = -1;  // loop counter
    double error = 999;
    double desiredThrust = 0;
    
    ofstream file;
    file.open(fileName.c_str());
    
    // Setup Moving Average Filter
    int n1 = 11; // Gyro
    int n2 = 20; // MAG
    int n3 = 5;  // CSS
    dataAQ GYRO(n1);
    dataAQ MAG1(n2);
    dataAQ MAG2(n2);
    dataAQ MAG3(n2);
    dataAQ CSS1(n3);
    dataAQ CSS2(n3);
    dataAQ CSS3(n3);
    dataAQ CSS4(n3);
    
    // Start the timer
    timer_settime(timerid, 0, &timer, NULL);
    
    while (1) {
        
        MsgReceive(chan_id, &pulse, sizeof (pulse), NULL);
        i += 1;

        // After maintain target speed for tartget time,
        // set tartget speed to 0, and return flag to 0
        if (start > desiredTime*freq) {
            flag = 0;
            TargetVolts = 2.38;
        }
        
        // Only start count after reaching the target speed
        if (start >= 0) start += 1;
        
        
        if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
        {
            fprintf(stderr, "%s%s\n", ERROR_PREFIX, dscGetErrorString(result) );
            free(samples);
            return -1;
        }
        
        float nowtime = i/freq;
        
        // UPdate Sensor readings
        SensorReadings.TimeStamp = nowtime;
        SensorReadings.GyroVolts =
        (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
        SensorReadings.MagXVolts =
        (double) (((short) dscadscan.sample_values[1] + 32768)/65536.0 * 5.0);
        SensorReadings.MagYVolts =
        (double) (((short) dscadscan.sample_values[2] + 32768)/65536.0 * 5.0);
        SensorReadings.MagZVolts =
        (double) (((short) dscadscan.sample_values[3] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS1Volts =
        (double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS2Volts =
        (double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS3Volts =
        (double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS4Volts =
        (double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);
        
        GYRO.dataPush(SensorReadings.GyroVolts);
        MAG1.dataPush(SensorReadings.MagXVolts);
        MAG2.dataPush(SensorReadings.MagYVolts);
        MAG3.dataPush(SensorReadings.MagZVolts);
        CSS1.dataPush(SensorReadings.CSS1Volts);
        CSS2.dataPush(SensorReadings.CSS2Volts);
        CSS3.dataPush(SensorReadings.CSS3Volts);
        CSS4.dataPush(SensorReadings.CSS4Volts);
        
        // Command Fan
        error = SensorReadings.GyroVolts - TargetVolts;
        
        // If reaching the target speed, start the counter
        if ((abs(error) < 0.05) && (start<0)) {
            start = 0;
        }
        
        // If flag is 0 and error  is small than 0.05,
        // the whole process is over and program
        // should return
        if ((!flag) && (abs(error) < 0.05)) {
            file.close();
            dscFree();
            return 0;
        }
        
        // If flag is not zero, TableSat in still in the
        // process of maintain the target speed. Otherwise
        // just set desiredThrust to 0
        if (flag) {
            desiredThrust =  error/abs(error)*8.2 + error * P;
        } else {
            desiredThrust = 0;
        }
        
        if (abs(desiredThrust) > 10)
            desiredThrust = abs(desiredThrust)/desiredThrust*10;
        if (abs(desiredThrust) < 8)
            desiredThrust = 0;
        
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
        
        // If freq is 100Hz, we are doing rate_control
        // else we are doing data acquisition
        if (freq == 100) {
            file << i/freq << " "\
            << SensorReadings.GyroVolts << " "\
            << desiredThrust << endl;
        } else {
            // Write Data
            if (!MA) {
                file << i/freq << " "\
                << SensorReadings.GyroVolts << " "\
                << SensorReadings.MagXVolts << " "\
                << SensorReadings.MagYVolts << " "\
                << SensorReadings.MagZVolts << " "\
                << SensorReadings.CSS1Volts << " "\
                << SensorReadings.CSS2Volts << " "\
                << SensorReadings.CSS3Volts << " "\
                << SensorReadings.CSS4Volts << " "\
                << endl;
            } else {
                file << i/freq << " "\
                << GYRO.MA() << " "\
                << MAG1.MA() << " "\
                << MAG2.MA() << " "\
                << MAG3.MA() << " "\
                << CSS1.MA() << " "\
                << CSS2.MA() << " "\
                << CSS3.MA() << " "\
                << CSS4.MA() << " "\
                << endl;
            }
        }
    }
    file.close();
    dscFree();
    return 1;
}

/********************************************************/
// Member Function point()
// Given target angle, set the TableSat heading
// to the angle.
// Note: The calibration function for Gyro should be
//       manually changed. Modify the parameter slope
//       and bias.
/********************************************************/

int qnxSystem::point(double target, int dataWrite, int test, string fileName) {
    double slope = -0.79448;        // Should be updated manually
    double bias = 1.8884;           // Should be updated manually
    double curAngle, curVelocity;
    double error = 360;
    double error_acc = 0;
    double desiredThrust = 0;
    double i = 0;
    
    ofstream file;
    file.open(fileName.c_str());
    
    int n1 = 11; // Gyro
    int n2 = 11; // MAG
    int n3 = 5;  // CSS
    dataAQ GYRO(n1);
    dataAQ MAG1(n2);
    dataAQ MAG2(n2);
    dataAQ MAG3(n2);
    dataAQ CSS1(n3);
    dataAQ CSS2(n3);
    dataAQ CSS3(n3);
    dataAQ CSS4(n3);
    
    // Start the timer
    timer_settime(timerid, 0, &timer, NULL);
    
    while (1) {
	
        MsgReceive(chan_id, &pulse, sizeof (pulse), NULL);
        i += 1;


        if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
        {
            fprintf(stderr, "%s%s\n", ERROR_PREFIX, dscGetErrorString(result) );
            free(samples);
            return -1;
        }


        SensorReadings.TimeStamp = i/freq;
        SensorReadings.GyroVolts =
        (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
        SensorReadings.MagXVolts =
        (double) (((short) dscadscan.sample_values[1] + 32768)/65536.0 * 5.0);
        SensorReadings.MagYVolts =
        (double) (((short) dscadscan.sample_values[2] + 32768)/65536.0 * 5.0);
        SensorReadings.MagZVolts =
        (double) (((short) dscadscan.sample_values[3] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS1Volts =
        (double) (((short) dscadscan.sample_values[4] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS2Volts =
        (double) (((short) dscadscan.sample_values[5] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS3Volts =
        (double) (((short) dscadscan.sample_values[6] + 32768)/65536.0 * 5.0);
        SensorReadings.CSS4Volts =
        (double) (((short) dscadscan.sample_values[7] + 32768)/65536.0 * 5.0);
        
        GYRO.dataPush(SensorReadings.GyroVolts);
        MAG1.dataPush(SensorReadings.MagXVolts);
        MAG2.dataPush(SensorReadings.MagYVolts);
        MAG3.dataPush(SensorReadings.MagZVolts);
        CSS1.dataPush(SensorReadings.CSS1Volts);
        CSS2.dataPush(SensorReadings.CSS2Volts);
        CSS3.dataPush(SensorReadings.CSS3Volts);
        CSS4.dataPush(SensorReadings.CSS4Volts);
       
        
        
        // Estimate current angle and angular velocity
        // Calculate error and desiredThrust
        curAngle = getAngleMag(MAG1.MA(), MAG2.MA());
        curVelocity = slope * SensorReadings.GyroVolts + bias;
        error = curAngle - target;
        
        // If error and velocity is small enough. Stop and return.
        if ((abs(error) < 2) && (abs(curVelocity) < 0.02)) {
            output_code = (unsigned short) (4095*(0/10.0));
            da_write(1, output_code);
            da_write(3, output_code);
            file.close();
            dscFree();
            return 1;
        }
        
        // Always moves in a shorter arc
        if (abs(error) > 180)
            error = -abs(error) / error * (360 - abs(error));
        

        error_acc += (error/180*3.1415926) / freq;
        //cout << "test1 error: " << error << endl;
        //cout << "test1 scaled error: " << (error/180*3.1415926) << endl;
        desiredThrust = P * (error/180*3.1415926) + I * error_acc + D * curVelocity;
        
        //cout << "P term: " << P * (error/180*3.1415926) << endl;
        //cout << "I term: " << I * error_acc << endl;
        //cout << "D term: " << D * curVelocity << endl;

        desiredThrust = abs(desiredThrust) / desiredThrust * 7.4 + desiredThrust;
        //cout << "test3 thrust: " << desiredThrust << endl;

        if (abs(desiredThrust) > 8.5)
            desiredThrust = abs(desiredThrust)/desiredThrust*8.5;
        if (abs(desiredThrust) < 7.5)
            desiredThrust = 0;
        
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
        
        // In test is true, print system status for debug use
        if (test) {
            cout << "Current Angle:" << curAngle << endl;
            cout << "Current Velocity: " << curVelocity << endl;
            cout << "Error = :" << error << endl;
            cout << "desireThrust = :" << desiredThrust << endl;
        }
        
        if (dataWrite) {
            file << i/freq << " "\
            << curAngle << " "\
            << curVelocity << " "\
            << desiredThrust << " "\
            << endl;
        }
        
    }
}

/********************************************************/
// Helper Function cali()
// Given filename of calibration function, readin the
// coefficents and save as a vector<vector<double> >
// Note: Magnetometer Calibration is used here
/********************************************************/

int qnxSystem::cali(string fileName) {
    
    ifstream file(fileName.c_str());
    string s;
    while (getline(file,s)) {
        vector<double> temp;
        istringstream sin(s);
        double tmp;
        while(sin>>tmp){
            temp.push_back(tmp);
            cout << tmp << " ";
        }
        cout << endl;
        pmag.push_back(temp);
    }
    return 0;
    
}

/********************************************************/
// Helper Function getAngleCss()
// Estimate current heading angle based on previous
// estimation and current CSS snesor readings
/********************************************************/

double qnxSystem::getAngleCss(double v1, double v2, double v3, double v4) {
    double result = 0;
    double css1 = 0;
    double css2 = 0;
    double css3 = 0;
    double css4 = 0;
    
    for (int i=0;i<8;i++) {
        css1 += pcss[0][i] * pow(v1, 7-i);
        css2 += pcss[1][i] * pow(v2, 7-i);
        css3 += pcss[2][i] * pow(v3, 7-i);
        css4 += pcss[3][i] * pow(v4, 7-i);
    }

    if ((css1 >= 0) && (css1<=125) && (v1 >= 0) && (v1 <= 2)) {
        if ((abs(bestAngle-css1) < 5) || (bestAngle < 0) || (abs(bestAngle-css1) > 350))
            return css1;
        else
            result = 0;
    }
    if ((css2 > 120) && (css2<=225) && (v2 >= 0) && (v2 <= 2)) {
        if ((abs(bestAngle-css2) < 5) || (bestAngle < 0) || (abs(bestAngle-css2) > 350))
            return css2;
        else
            result = 0;
    }
    if ((css4 > 220) && (css4<=305) && (v4 >= 0) && (v4 <= 2)) {
        if ((abs(bestAngle-css4) < 5) || (bestAngle < 0) || (abs(bestAngle-css3) > 350))
            return css4;
        else
            result = 0;
    }
    if ((css3 > 300) && (css3<=359.999) && (v3 >= 0) && (v3 <= 2)) {
        if ((abs(bestAngle-css3) < 5) || (bestAngle < 0) || (abs(bestAngle-css4) > 350))
            return css3;
        else
            result = 0;
    }
    
    return bestAngle;
}

/********************************************************/
// Helper Function getAngleMag()
// Estimate current heading angle based on previous
// estimation and current MAG snesor readings
/********************************************************/

double qnxSystem::getAngleMag(double magX, double magY) {
    
    double start, range, angle;
    if (bestAngle < 0) {
        start = 0;
        range = 360;
    } else {
        start = bestAngle-5;
        range = 10;
    }
    int step = (int) range / 0.05;
    int count = 0;
    double min = 999;
    double diff = 0;
    
    while (count <= step) {
        count += 1;
        if (start < 0) start = start + 360;
        if (start >=360) start = start -360;
        diff = pow(magX - getFT(pmag[0], start), 2) + pow(magY - getFT(pmag[1], start), 2);
        if (diff <=  min) {
            min = diff;
            angle = start;
        }
        start += 0.05;
    }
    return angle;
}

/********************************************************/
// Helper Function getFT()
// Calulate Fourier Series Expansion based on
// MAG calibration file and angle.
/********************************************************/
double qnxSystem::getFT(vector<double> p, double angle) {
    double w = p[p.size()-1];
    int order = (p.size()-2) / 2;
    double result = p[0];
    for (int i = 1; i<order+1;i++) {
        result += p[2*i-1] * cos(i*angle*w) + p[2*i] * sin(i*angle*w);
    }
    
    return result;
}
