//LIDAR Related

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.

////////////////////////////////////////////////////////////////////////////////
//LED Related

#include <FastLED.h>

#define LED_PIN  5

#define COLOR_ORDER RGB
#define CHIPSET     WS2812B

#define BRIGHTNESS 255
#define NUM_LEDS 86
void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);

#define hue 171
#define sat 150

// Define the array of leds
CRGB leds[NUM_LEDS];

#define MAX_DIST 600
int _r, _g, _b;

int smooth(int data, float filterVal, float smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
float filterVal = 0.1;
 //  smoothedVal =  smooth(sensVal, filterVal, smoothedVal);   // second parameter determines smoothness  - 0 is off,  .9999 is max smooth 
int smoothedVal;
////////////////////////////////////////////////////////////////////////////////
byte prevled = 45;
boolean pass = false;
byte count = 8;
void displayColor(float angle, float distance)
{
  FastLED.clear();
  angle = constrain(angle, 1, 359);
  distance = constrain(distance, 0, MAX_DIST);

  byte val = map(distance, 0, MAX_DIST, 0, 255);
  byte led = map(angle, 1, 359, 0, NUM_LEDS - 1);
  
  //1. map 0-350 deg to 0-23
  //byte hue = angle*255/360;


  //2. control the light
  //    if(val==255){
  //      val=0;
  //    }

//  smoothedVal =  smooth(led, filterVal, smoothedVal);
//  led=smoothedVal;
  Serial.println(" ");
  Serial.println(val);
  Serial.println(led);

 if((abs(prevled-led)<10)||pass){
  if (led - 1 > 0) {
    leds[led - 1] = CHSV(hue, sat, 255 - val);
    if (led - 2 > 0) {
      leds[led - 2] = CHSV( hue, sat, 255 - val);
      if (led - 3 > 0) {
        leds[led - 3] = CHSV( hue, sat, 255 - val);
      }
    }
  }

  
  leds[led] = CHSV( hue, sat, 255 - val);
  
  if (led + 1 < NUM_LEDS) {
    leds[led + 1] = CHSV( hue, sat, 255 - val);
    if (led + 2 < NUM_LEDS) {
      leds[led + 2] = CHSV( hue, sat, 255 - val);
      if (led + 3 < NUM_LEDS) {
        leds[led + 3] = CHSV( hue, sat, 255 - val);
      }
    }
  }

    FastLED.show();
    delay(10);
    prevled=led;
    pass=false;
//    if(count=10000){
//      pass=true;
//     count=0;
//    }    count=count+1;
 }


//    if(distance>(MAX_DIST/2)){
//      pass=true;
        
//    }
 // }
}

  void setup() {
    Serial.begin(9600);

    //LIDAR
    lidar.begin(Serial1);
    pinMode(RPLIDAR_MOTOR, OUTPUT);
    analogWrite(RPLIDAR_MOTOR, 150);
    //LED
    LEDS.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS);
  }

  float minDistance = 100000;
  float angleAtMinDist = 0;
  float prevAngle = 0;

  void loop() {

    //  if (Serial.available()>0) {
    //  angleAtMinDist = Serial.parseInt();
    //  Serial.print("Angle:   ");
    //  Serial.println(angleAtMinDist);
    //
    //
    //  minDistance = Serial.parseInt();
    //  Serial.print("Dist:   ");
    //  Serial.println(minDistance);
    //  }
    //
    //
    //  if(prevAngle!=angleAtMinDist){
    //  displayColor(angleAtMinDist, minDistance);
    //  prevAngle=angleAtMinDist;
    //  }
    if (IS_OK(lidar.waitPoint())) {
      //perform data processing here...
      float distance = lidar.getCurrentPoint().distance;
      float angle = lidar.getCurrentPoint().angle;

      if (lidar.getCurrentPoint().startBit) {
        // a new scan, display the previous data...

        displayColor(angleAtMinDist, minDistance);

        minDistance = 100000;
        angleAtMinDist = 0;
      } else {
        if ( distance > 0 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
        }
      }
    } else {
      analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

      // try to detect RPLIDAR...
      rplidar_response_device_info_t info;
      if (IS_OK(lidar.getDeviceInfo(info, 100))) {
        //detected...
        lidar.startScan();
        analogWrite(RPLIDAR_MOTOR, 255);
        delay(1000);
      }
    }
  }
