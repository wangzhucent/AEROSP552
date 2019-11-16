#include "AE552_Project.h"

int main() {
    double p, i, d, angle;
    string cali, file;
    int test, dataWrite;
    
    // Readin Specifications
    cout << "Enter Target Angle(0-359.999):" << endl;
    cin >> angle;
    cout << "Enter Desired Gains(P, I, and D):" << endl;
    cin >> p >> i >> d;
    cout << "Enter Calibration Filename:" << endl;
    cin >> cali;
    cout << "Enter Data Filename:" << endl;
    cin >> file;
    cout << "Enable Data Writing for Report?(0 or 1):" << endl;
    cin >> dataWrite;
    cout << "Enable Debug Mode for Gain Tuning?(0 or 1):" << endl;
    cin >> test;
    
    qnxSystem dataAquisition(0, 0, 100, 1, p, i, d);
    dataAquisition.cali(cali);
    dataAquisition.point(angle, dataWrite, test, file);
    
}
