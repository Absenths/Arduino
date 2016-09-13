
#include "FastLED.h"

//LEDS  
#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 15
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];



int sensorPin = A4;  
int relay = 4;      
int sens = 0;

int sensorLimit = 500;
int lsrLvl = 0;
int shotTime = 2000;
int fillUpSpeed = 400;

int lsr[15];

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(relay, OUTPUT);  
  digitalWrite(relay,HIGH);

  // For mirroring strips, all the "special" stuff happens just in setup.  We
  // just addLeds multiple times, once for each strip

  // tell FastLED there's 60 NEOPIXEL leds on pin 10
  FastLED.addLeds<NEOPIXEL, 10>(leds[0], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 11
  FastLED.addLeds<NEOPIXEL, 11>(leds[1], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 12
  FastLED.addLeds<NEOPIXEL, 12>(leds[2], NUM_LEDS_PER_STRIP);

  


}

void loop() {
  // read the value from the sensor:

  while(lsrLvl<15){
    
    sens = analogRead(sensorPin);
    if(sens>sensorLimit){
      lsrLvl++;
    }
    else if(lsrLvl!=0){
      lsrLvl--; 
    }
    Serial.println(sens);

    for(int x = 0; x < NUM_STRIPS; x++) {
      for(int i = 0; i < lsrLvl; i++) {
        leds[x][i] = CRGB::Red;

      }
    }

    FastLED.show();
    delay(fillUpSpeed);

    allLedsBlack(); 
  }
  
  
    if(lsrLvl=15){
      Serial.println("Bose in coming!"); 
      allLedsGreen();
      FastLED.show();
      digitalWrite(relay,LOW);
      delay(shotTime);
      digitalWrite(relay,HIGH);
      allLedsBlack();
      FastLED.show();
      lsrLvl = 0;

    }

}


void allLedsBlack(){
  for(int x = 0; x < NUM_STRIPS; x++) {
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {

      leds[x][i] = CRGB::Black;

    }
  }

}

void allLedsGreen(){
  for(int x = 0; x < NUM_STRIPS; x++) {
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {

      leds[x][i] = CRGB::Green;

    }
  }

}

