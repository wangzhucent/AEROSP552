// Program to drive TableSat thrusters (fans)
//
// To build program on QNX (mfg-88):
//    gcc command_fan.c da.c -o command_fan -ldscud5 -lm
//

#include <stdio.h>
#include "da.h"
#include <dscud.h>


int main() 
{
  int i;
  unsigned short output_code;
  double xblahblah;
  double desiredThrust=0.0;

  // Initialize Prometheus hardware
  if (da_init()) return(-1);
  
  for (i=0;i<5;i++) {  
    printf("Enter desired thrust (-10 to 10V):\n");
    scanf("%lf", &desiredThrust);
    
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
  }
  dscFree();
  
  return(0);
}
