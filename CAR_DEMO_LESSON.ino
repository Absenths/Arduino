void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT);  //Initiates Brake Channel A pin

  

  digitalWrite(12, HIGH);  //Establishes direction of rotation
  digitalWrite(9, LOW);   //Disengage the Brake
}

void loop() {
  // put your main code here, to run repeatedly:

  int sped = analogRead(A5);
  
  analogWrite(3, sped/4);

  //delay(40);
}
