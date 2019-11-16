#include "project1.h"

int main() 
{
  
  int i;
  double desiredThrust=0.0;
  // Initialize Prometheus hardware
  if (da_init()) return(-1);
  
  for (i=0;i<5;i++) {  
    printf("Enter desired thrust (-10 to 10V):\n");
    scanf("%lf", &desiredThrust);
    
    // Set thrust levels
    fan_speed(desiredThrust);
  }
  dscFree();
  
  return(0);
}
