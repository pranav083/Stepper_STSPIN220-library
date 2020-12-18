/*
  Stepper_STSPIN220.cpp - - Arduino library for using the Stepper_STSPIN220 stepper driver
  Pranav Kumar, 2020
  email = kpranav083@gmail.com
  See the Datasheet: https://www.st.com/resource/en/datasheet/stspin220.pdf

  The Stepper_STSPIN220 stepper driver is for Pololu stepper driver boards
  and compatible clones. These boards use the Allegro Stepper_STSPIN220
  stepper motor driver IC. (see Allegro website for datasheet)

  This library diverges from others that are around, in that it
  assumes that the MS1, MS2, MS3(DIR) and MS4(STEP) pins are connected to gpio
  pins on the Arduino, allowing control over the microstepping
  modes.
  
  The Stepper_STSPIN220 is capable of microstepping down to 1/256 of a step,
  enabling fine control over the stepper motor. This fine control
  can be used in, among other things, 3D printers.

  This library provides an interface for setting the different
  step modes, going from full step down to 1/256 step, using a
  simple setter function, where the argument is 1, 2, 4, 8, 16, 32, 64, 128 or 256.

  At just after the reset or wake up from standby the device is then only able to 
  set the microstepping mode. 

  SL NO.   MODE4  MODE3  MODE2  MODE1  Step mode 
           (DIR)  (STCK)                                   
     1       0       0      0     0     Full      step
     2       0       1      0     1     1/2       step
     3       1       0      1     0     1/4th     step
     4       1       1      0     1     1/8th     step
     5       1       1      1     1     1/16th    step
     6       0       0      1     0     1/32nd    step
     7       1       0      1     1     1/64th    step
     8       0       0      0     1     1/128th   step
     9       0       0      1     1     1/256th   step  

  Note:
  Lower delay values can be used in the microstepping mode.
  Values as low as 25 usec can be used in the 1/256 mode
  with some motors. 

*/

#include "Arduino.h"
#include "Stepper_STSPIN220.h"

// constructor to set up pins and initialize data
Stepper_STSPIN220::Stepper_STSPIN220(unsigned long motor_steps, int ms1_pin, int ms2_pin, int ms3_pin, int ms4_pin, int enable_pin = -1)
{
   this->enable_pin = enable_pin;
   this->ms1_pin = ms1_pin;
   this->ms2_pin = ms2_pin;
   this->ms3_pin = ms3_pin;     // Step
   this->ms4_pin = ms4_pin;     // Dir 

   // setup the pins on the microcontroller:
   pinMode(this->ms1_pin, OUTPUT);
   pinMode(this->ms2_pin, OUTPUT);
   pinMode(this->ms3_pin, OUTPUT);      // Step
   pinMode(this->ms4_pin, OUTPUT);      // Dir 
   if(enable_pin != -1){pinMode(this->enable_pin, OUTPUT);}
   
   if(motor_steps != 0)
   {
      this->motor_steps = motor_steps; 
   }
   else
   {
      this->motor_steps = 200;       // a common value for steppers
   }

   // use setDelay to change before stepping, otherwise default
   this->step_delay = 2000;         // 20000 us (20 ms) as a default
                                     
}

void Stepper_STSPIN220::enable(int enable)
{
   digitalWrite(this->enable_pin, enable);  // set enable pin on/off
}

// set delay in microseconds, set before starting to step
void Stepper_STSPIN220::setDelay(unsigned long delay)
{
   this->step_delay = delay;
}

// set direction: 0 or 1
void Stepper_STSPIN220::setDirection(int direction)
{
   if(direction == 0)
   {
      digitalWrite(ms4_pin,HIGH);       // Dir
   }
   else 
   {
      digitalWrite(ms4_pin,LOW);        // Dir
   }
}

// step only once
void Stepper_STSPIN220::stepOnce(void)
{
    digitalWrite(ms3_pin, HIGH);        // Step
    delayMicroseconds(this->step_delay);
    digitalWrite(ms3_pin, LOW);         // Step
    delayMicroseconds(this->step_delay);
}

// step given number of times
void Stepper_STSPIN220::step(unsigned long num_steps)
{
   unsigned long x;
   for(x = 0; x < num_steps; x++)
   {
      this->stepOnce();
   }
}

/*
 * Sets the speed in revs per minute
 */
void Stepper_STSPIN220::setSpeed(long Speed)
{
  this->step_delay = 60L * 1000L * 1000L / this->motor_steps / Speed;
}

//   version() returns the version of the library:
int Stepper_STSPIN220::version(void)
{
  return 1;
}
/*
  SL NO.   MODE4  MODE3  MODE2  MODE1  Step mode 
           (DIR)  (STCK)                                   
     1       0       0      0     0     Full      step
     2       0       1      0     1     1/2       step
     3       1       0      1     0     1/4th     step
     4       1       1      0     1     1/8th     step
     5       1       1      1     1     1/16th    step
     6       0       0      1     0     1/32nd    step
     7       1       0      1     1     1/64th    step
     8       0       0      0     1     1/128th   step
     9       0       0      1     1     1/256th   step  
   set microstepping mode
   1, 2, 4, 8, 16, 32, 64, 128 or 256.
   0 = LOW
   1 = HIGH
*/
void Stepper_STSPIN220::setStepMode(int stepMode)
{   
    this->step_mode = stepMode;
   //  this->motor_steps = motor_steps * stepMode;   // for microstepping to update the total step count
    switch (stepMode)
    {
                  //  MODE4   MODE3  MODE2 MODE1
      case 1:     //    0       0      0     0
      digitalWrite(ms1_pin, 0);
      digitalWrite(ms2_pin, 0);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 0);
      break;                  
      case 2:     //    0       1      0     1  
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 0);
      digitalWrite(ms3_pin, 1);
      digitalWrite(ms4_pin, 0);
      break;                  
      case 4:     //    1       0      1     0  
      digitalWrite(ms1_pin, 0);
      digitalWrite(ms2_pin, 1);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 1);
      break;                  
      case 8:     //    1       1      0     1  
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 0);
      digitalWrite(ms3_pin, 1);
      digitalWrite(ms4_pin, 1);
      break;                  
      case 16:     //   1      1      1      1   
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 1);
      digitalWrite(ms3_pin, 1);
      digitalWrite(ms4_pin, 1);
      break;
      case 32:     //   0       0      1     0   
      digitalWrite(ms1_pin, 0);
      digitalWrite(ms2_pin, 1);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 0);
      break;                  
      case 64:     //   1       0      1     1   
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 1);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 1);
      break;                  
      case 128:     //  0       0      0     1    
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 0);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 0);
      break;                  
      case 256:     //  0      0      1      1    
      digitalWrite(ms1_pin, 1);
      digitalWrite(ms2_pin, 1);
      digitalWrite(ms3_pin, 0);
      digitalWrite(ms4_pin, 0);
      break;                  
    }
}
