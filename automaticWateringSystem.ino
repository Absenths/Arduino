//Low analog == Low water

// --------------------------------------------------------------------------- Motors
int pump1[] = {2, 3};
int pump2[] = {4, 5};
int pump3[] = {6, 7};

int sens1 = A0;
int sens2 = A1;
int sens3 = A2;

int sens1Limit = 700;
int sens2Limit = 700;
int sens3Limit = 700;

boolean sensPwr1 = LOW;
boolean sensPwr2 = LOW;
boolean sensPwr3 = LOW;

int sensPwrPin1 = 8;
int sensPwrPin2 = 9;
int sensPwrPin3 = 10;

// --------------------------------------------------------------------------- Setup
void setup() {
Serial.begin(9600);

// Setup pumps
int i;
for(i = 0; i < 2; i++){
pinMode(pump1[i], OUTPUT);
pinMode(pump2[i], OUTPUT);
pinMode(pump3[i], OUTPUT);
}

pinMode(sensPwrPin1, OUTPUT);
pinMode(sensPwrPin2, OUTPUT);
pinMode(sensPwrPin3, OUTPUT);

digitalWrite(sensPwrPin1, sensPwr1);
digitalWrite(sensPwrPin2, sensPwr2);
digitalWrite(sensPwrPin3, sensPwr3);


}

// --------------------------------------------------------------------------- Loop
void loop() {
  
  if(poll(1)){
   drivePump1(300); 
  }
    
  if(poll(2)){
   drivePump2(400);
  }
  
  if(poll(3)){
   drivePump3(500); 
  }
  

}


boolean poll(int nbr){
 int sens;
 boolean dry = LOW;
 switch (nbr) {
    case 1:
      sensPwr1=HIGH;
      digitalWrite(sensPwrPin1,sensPwr1);
      delay(50);
      sens=analogRead(sens1);
      Serial.println(sens);
      if(sens<sens1Limit){
        dry = HIGH;
      }
      break;
    case 2:
      sensPwr2=HIGH;
      digitalWrite(sensPwrPin2,sensPwr2);
      delay(50);
      sens=analogRead(sens2);
      Serial.println(sens);
      if(sens<sens2Limit){
        dry = HIGH;
      }      //do something when var equals 2
      break;
    case 3:
      sensPwr3=HIGH;
      digitalWrite(sensPwrPin3,sensPwr3);
      delay(50);
      sens=analogRead(sens3);
      Serial.println(sens);
      if(sens<sens3Limit){
        dry = HIGH;
      }      //do something when var equals 2
      break;
  } 
  Serial.println(dry);
  return dry;
}

void drivePump1(int time){
  digitalWrite(pump1[0],LOW);
  digitalWrite(pump1[1],HIGH);
  delay(time);
  digitalWrite(pump1[0],LOW);
  digitalWrite(pump1[1],LOW);
  
}


void drivePump2(int time){
  digitalWrite(pump2[0],LOW);
  digitalWrite(pump2[1],HIGH);
  delay(time);
  digitalWrite(pump2[0],LOW);
  digitalWrite(pump2[1],LOW);  
}


void drivePump3(int time){
  digitalWrite(pump3[0],LOW);
  digitalWrite(pump3[1],HIGH); 
  delay(time);
  digitalWrite(pump3[0],LOW);
  digitalWrite(pump3[1],LOW);  
}


