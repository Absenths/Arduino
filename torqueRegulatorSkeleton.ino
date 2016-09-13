double Kt; // Torque vs Current Constant (T = Kt*I)
double Ki; // Angular momentum vs Back Electro Motive Force Constant (Vemf = w*Ki)

int posRef = 0; // Angle for home position
int pos = 0; // Current angle
int Tref // Torque for home position (according to lookup table)
int Treal // Torque for current position (given from current angle via look-up table)
int Terror // Difference in torque (the error for the regulator)
void setup() {
  Serial.begin(9600);
  // Här ska pinModes deklareras

}

void loop(){


  Läser av och returnerar positionen för ratten
    while (someone is holding the wheel) { // "Compiant Positioning" is active and hands are on the wheel 
    posRef = getPos();    // Saves the position (current angle)
    Tref = torque(posRef); // Gets the value of torque from gravity at angle posRef from a lookup table 
    Vpwm = TorqueToVpwm(Tref);  // transfers torque to corresponding PWM value (voltage) 
    analogWrite(Vpwm);  // Sends PWM Voltage to motor
    delay(10);  // delay for stability
  }

  while ((someone is holding the wheel)!) { // "Compiant Positioning" is active but hands are not on the wheel  
    pos=getPos();
    Treal = torque(pos); // Get the torque needed to compensate for the measured position from lookup table
    Terror = Treal - Tref; // Finds the error in torque
    Vpwmerror = felRef(Terror); // Calculates the voltage needed to compensate the error (and only the error)
    Vpwref = TorqueToVpwm(Tref); // Calculates the voltage needed to compensate the error (and only the error)
    analogWrite(Vpwmref + Vpwmerror) // Skickar totalspänning till motorn
    }


  }
  
  
int getpos() { //Reads and returns position (angle) of the wheel
    analogRead(A3);
  }
  
int torque(int pos) { //Looks up torque for given value from equation/table 
}
int TorqueToVpwm(int torque) { //Convertes desired torque to PWM value
}
int felReg(int torque) { //Regulator for the error
}





