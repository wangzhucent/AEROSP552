#include "AE552_Project.h"

int main() {
    double volts, time;
    int freq, mode;
    string file;
    
    // Readin Specifications
    cout << "Enter Target Volt(0-5V): " << endl;
    cin >> volts;
    cout << "Enter Target Frequency(Hz): " << endl;
    cin >> freq;
    cout << "Enter Target Time(s): " << endl;
    cin >> time;
    cout << "Enter Target Filename:" << endl;
    cin >> file;
    cout << "Using Moving Average filter?(0 or 1):" << endl;
    cin >> mode;
    
    qnxSystem dataAquisition(volts, time, freq, mode, 4, 0, 0);
    dataAquisition.daq(file);
    
}
