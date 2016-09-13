
//Define the position where you want the shaft
#define zeroPos 500
//Variable for analog input (angle of shaft)
int realPos = 500;

void setup() {
  Serial.begin(9600);  
  //Setup Channel A for motor
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT);  //Initiates Brake Channel A pin
  pinMode(A2, INPUT);
}

void loop(){
  //Read position of shaft
  realPos = analogRead(A2);
  Serial.print(realPos);

  //Determine if the shaft is moved
  if(realPos < zeroPos -5){
    //Shaft is moved clockwise (needs anti-clockwise movement to return to zero)
    Serial.println("  -->  Shaft will move anti-clockwise"); 
    moveBack(LOW);
      delay(30);
  }
  else if(realPos > zeroPos + 5){

    //Shaft is moved anti-clockwise (needs clockwise movement to return to zero)
    Serial.println("  -->  Shaft will move clockwise"); 
    moveBack(HIGH);
      delay(30);


  }
  else{
    digitalWrite(9, HIGH);  //Engage Brake
    Serial.println("  -->  Shaft in position"); 
    delay(10);
  }


}

void moveBack(boolean dir){

  realPos = analogRead(A2);
  int mode = abs(realPos-zeroPos); 

  digitalWrite(12, dir);  //Establishes direction of rotation
  digitalWrite(9, LOW);   //Disengage the Brake 

  if(mode>50){
    analogWrite(3,170);     //PWM signal to motor
    Serial.print(    "Speed: Fast"); 
  }
  else{
    analogWrite(3,60);     //PWM signal to motor    
    Serial.print(    "Speed: Slow"); 
  }
  
}



