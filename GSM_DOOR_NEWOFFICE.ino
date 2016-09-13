/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

// change this to the number of steps on your motor: 1.8deg = 200
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to  A+, A-, B+ and B- out 1234
Stepper stepper(STEPS, 11, 10, 9,8);
// the previous reading from the analog input
int previous = 0;
bool locked = true;
void setup() {
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(15);
  Serial.begin(9600);
    pinMode(2, INPUT);
    pinMode(13,OUTPUT);
  attachInterrupt(0, openOrClose, RISING); 
}

void loop() {
  Serial.println("looping");
 //stepper.step(60);
  delay(4000);

   }

void openOrClose(){
  static unsigned long last_time = 0;
  unsigned long int_time = millis();
  if(int_time - last_time > 1000){
  if(locked){
    //stepper.step(50);
    Serial.println("open");
    digitalWrite(13,locked);
    //stepper.step(50);
    locked=!locked;
    delay(50);
  }else{
    Serial.println("closed");
        digitalWrite(13,locked);
    //stepper.step(-50);
    locked=!locked;
    delay(50);
  }
  last_time = int_time;
  }
  
}
