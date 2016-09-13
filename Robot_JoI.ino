/*
HC-SR04 Ping distance sensor]
 VCC to arduino 5v GND to arduino GND
 Echo to Arduino pin 13 Trig to Arduino pin 12
 Red POS to Arduino pin 11
 Green POS to Arduino pin 10
 560 ohm resistor to both LED NEG and GRD power rail
 More info at: http://goo.gl/kJ8Gl
 Original code improvements to the Ping sketch sourced from Trollmaker.com
 Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
 */

#define trigPin 13
#define trigPin2 9
#define echoPin 12
#define echoPin2 8
#define led 11
#define led2 10

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);  
  pinMode(echoPin2, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;


  if (distance < 10) {  // This is where the LED On/Off happens
    stopp();

    //Vägg hittad framför - Kollar avsånd åt vänster
    Serial.println("wall ahead");
    digitalWrite(trigPin2, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin2, HIGH);
    //  delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin2, LOW);
    duration = pulseIn(echoPin2, HIGH);
    distance = (duration/2) / 29.1;

    if(distance < 10) { 
      //sväng höger
      stopp();
      Serial.println("Turn Right stop");
    }
    else{
      //turn left
      goBack();
      Serial.println("Turn Left Go Back");
    }

  }
  else {
    goForward();
    Serial.println("GO");
  }
  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}
//Methods
void goForward(){
  digitalWrite(led,LOW);
  digitalWrite(led2,HIGH);
}
void stopp(){
  digitalWrite(led,LOW);
  digitalWrite(led2,LOW);
}
void goBack(){
  digitalWrite(led,HIGH);
    digitalWrite(led2,LOW);
}
