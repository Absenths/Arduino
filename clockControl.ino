int second = 1000*60;
boolean bol = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(3,bol);
  digitalWrite(4,!(bol));
  delay(1000);
  bol = !(bol);

  
}
