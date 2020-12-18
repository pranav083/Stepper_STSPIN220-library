#include <Arduino.h>
#include "Stepper_STSPIN220.h"

int MODE_1 =  6;                    // Pin 6
int MODE_2 =  7;                    // Pin 7
int MODE_3 =  8;                    // Step Pin8
int MODE_4 =  9;                    // Dir Pin 9
int microstepping = 64;             // set microstepping mode   1, 2, 4, 8, 16, 32, 64, 128 or 256.
unsigned long MOTOR_STEPS = 200;    // Total no steps per revolution for motor in full step
unsigned long MOTOR_STEPS_TOTAL = MOTOR_STEPS * long(microstepping); // Total no. of motor steps
int EN     = -1;                    // no pin defined

int speed  = 10;                    //either set speed or give delay  
unsigned long stepdelay = 2000;     // Microseconds delay  // Higher# = more rapid changes between positions, lower = softer transitions

Stepper_STSPIN220 stepper1(MOTOR_STEPS_TOTAL, MODE_1, MODE_2,MODE_3, MODE_4, -1); // Driver PULL/STEP wire to pin = 8, Driver DIR wire to pin = 9

void setup() {
  Serial.begin(9600);
  stepper1.setStepMode(microstepping);  // set microstepping and update total no. of steps
  stepper1.setDelay(stepdelay);
  delay(1000);                          // Wait for the driver to latch the values
}           
                               
void loop() {
  Serial.println("");
  Serial.println("START LOOP"); 
  stepper1.setDirection(0);                                     
  stepper1.setSpeed(speed);
  stepper1.step(2000);                  //use this to move to a fixed position in steps.
  delay(3000);
  stepper1.setDirection(1);
  stepper1.setSpeed(speed);
  stepper1.step(1000);                  //use this to move to a fixed position in steps.
  delay(3000);
}
