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

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64
#define NUM_LEDS 24

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);

// Define the array of leds
CRGB leds[NUM_LEDS];

#define NEO_RGBSPACE_MAX (byte)(200L*255/360)
int _r, _g, _b;



////////////////////////////////////////////////////////////////////////////////
void displayColor(float angle, float distance)
{

    angle = constrain(angle,1,359);
    distance = constrain(distance,0,500);

    byte val = map(distance,0,500,0,255);
    byte hue = map(angle,1,359,0,255);
    
    //1. map 0-350 deg to 0-23
    //byte hue = angle*255/360;
   
    
    //2. control the light 
//    if(val==255){
//      val=0;
//    }


    Serial.println(" ");
    Serial.println(val);

    Serial.println(hue);
    
    leds[15] = CHSV( hue, 187, 255-val);
FastLED.show();
delay(50);

}

void setup() {
  Serial.begin(9600);
  
  //LIDAR
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
       analogWrite(RPLIDAR_MOTOR, 150);
  //LED
    LEDS.addLeds<CHIPSET,LED_PIN,RGB>(leds,NUM_LEDS);
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
