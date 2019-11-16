#include "AE552_Project.h"

int main() {
    double volts;
    int freq;
    string file;
    
    // Readin Specifications
    cout << "Enter Target Volt(0-5V): " << endl;
    cin >> volts;
    cout << "Enter Target Filename:" << endl;
    cin >> file;
    
    qnxSystem dataAquisition(volts, 30, 100, 0, 5, 0, 0);
    dataAquisition.daq(file);
    
}
