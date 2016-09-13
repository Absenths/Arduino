void setup() {
 pinMode (7,OUTPUT);
 Serial.begin(9600);

}

void loop() { 

digitalWrite(7,LOW); // Turns ON Relays 1

Serial.println("Light ON");

delay(2000); // Wait 2 seconds

digitalWrite(7,HIGH); // Turns Relay Off

Serial.println("Light OFF");

delay(2000);

}

