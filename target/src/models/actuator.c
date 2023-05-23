#include <rc_servo.h>
#include "actuator.h"


void initServo(){
    //init servo
    rc_servo_initialise();
}

void setServoHigh(){
    
    rc_servo_setPosition(0,100);
}
void setServoLow(){
   
    rc_servo_setPosition(0,-100);
}
void setServoOptimal(){
   
    rc_servo_setPosition(0,0);
}



