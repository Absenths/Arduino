const int
PWM_A   = 3,
DIR_A   = 12,
BRAKE_A = 9,
SNS_A   = A0,
ANGLE_A = A2,
ANGLE_B = A3,
TORQUE_TO_PWM = 0,
VOLT_TO_RAD = 0,
M1 = 0, 
M2 = 0,
M3 = 0,
M4 = 0,
L1 = 0,
L2 = 0;
void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Open Serial communication
  Serial.begin(9600);
  Serial.println("Motor shield DC motor Test:\n");
}

void loop() {

  Serial.print("Kalibrering");

  digitalWrite(BRAKE_A,LOW);
  digitalWrite(DIR_A,HIGH);
  Serial.print("Kör frammåt");
  for(int i = 1; i<11;i++){
    analogWrite(PWM_A, 25*i);
    Serial.print(" PWM är " + 25*i);
    delay(5000);
    
  }

 digitalWrite(DIR_A,LOW);
  Serial.print("Kör bakåt");
  for(int i = 1; i<11;i++){
    analogWrite(PWM_A, 25*i);
    Serial.print(" PWM är " + 25*i);
    delay(5000);
    
  }
  
 // analogWrite(PWM_A, (TORQUE_TO_PWM*(M4/2+M2)*L2*G*cos(VOLT_TO_RAD*analogRead(ANGLE_A));
  
//// Set the outputs to run the motor forward
//  
//  digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
//  digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
//
//  analogWrite(PWM_A, 255);     // Set the speed of the motor, 255 is the maximum value
//
//  delay(5000);                 // hold the motor at full speed for 5 seconds
//  Serial.print("current consumption at full speed: ");
//  Serial.println(analogRead(SNS_A));
//
//// Brake the motor
//
//  Serial.println("Start braking\n");
//  // raising the brake pin the motor will stop faster than the stop by inertia
//  digitalWrite(BRAKE_A, HIGH);  // raise the brake
//  delay(5000);
//
//// Set the outputs to run the motor backward
//
//  Serial.println("Backward");
//  digitalWrite(BRAKE_A, LOW);  // setting againg the brake LOW to disable motor brake
//  digitalWrite(DIR_A, LOW);    // now change the direction to backward setting LOW the DIR_A pin
//
//  analogWrite(PWM_A, 255);     // Set the speed of the motor
//
//  delay(5000);
//  Serial.print("current consumption backward: ");
//  Serial.println(analogRead(SNS_A));
//
//  // now stop the motor by inertia, the motor will stop slower than with the brake function
//  analogWrite(PWM_A, 0);       // turn off power to the motor
//
//  Serial.print("current brake: ");
//  Serial.println(analogRead(A0));
//  Serial.println("End of the motor shield test with DC motors. Thank you!");
//
//
//  while(1);
}
