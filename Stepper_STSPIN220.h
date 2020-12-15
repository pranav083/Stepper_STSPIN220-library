/*
  Stepper_STSPIN220.cpp - - Arduino library for using the Stepper_STSPIN220 stepper driver
  Pranav Kumar, 2020
  email = kpranav083@gmail.com
  
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

#ifndef Stepper_STSPIN220_h
#define Stepper_STSPIN220_h

class Stepper_STSPIN220 {
  public:
    // constructors:
    Stepper_STSPIN220(int motor_steps, int ms1_pin, int ms2_pin, int ms3_pin, int ms4_pin, int enable_pin);

    // speed setter method:
    void setDelay(unsigned long delay);   // in microseconds

    void enable(int enable);

    // return software version
    int version(void);
    // mover method:
    void setDirection(int direction);     // 0 forward, 1 backward
    void step(unsigned long num_steps);   // step a given number of steps
    void stepOnce(void);
    void setStepMode(int stepMode);       // 1, 2, 4, 8, 16, 32, 64, 128 or 256.

  private:
    void stepMotor(int this_step);

    
    int direction;               // Direction of rotation
    int motor_steps;             // number of steps motor has per revolution
    unsigned long step_delay;    // delay between steps, in us
    int num_steps;               // total number of steps to step
    int step_number;             // which step the motor is on
    int step_mode;               // which mode 1 / 1, 2, 4, 8, or 16
    int step_pin;                // pin which controls stepping

    // control pin numbers:
    int ms1_pin;                 //MODE1                        // for setting different microstepping modes
    int ms2_pin;                 //MODE2                        // for setting different microstepping modes
    int ms3_pin;                 // Step                        // for setting different microstepping modes
    int ms4_pin;                 // Dir                         // for setting different microstepping modes
    int enable_pin;
    
};

#endif //Stepper_STSPIN220_h