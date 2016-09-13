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

#define BRIGHTNESS 255
#define NUM_LEDS 53
void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);

//DEFINE_GRADIENT_PALETTE( heatmap_gp ) { //GREEN RED BLUE
//  0,   0,  0,  0,   //far away = black
//  5,    0  ,255,  0 ,   //RED
//150,   255,255,  0,   //
//255,   255  ,0,  0 }; //black
//CRGBPalette16 myPal = heatmap_gp;

DEFINE_GRADIENT_PALETTE( heatmap_gp ) { // RED GREEN BLUE
  0,   0  ,  0,  0,   //far away = black
  1,   0  ,255,  0 ,  //Green   5
 127,   255,255,  0,   //Yellow 50
 255,   255  ,0,  0 }; //Red
CRGBPalette16 myPal = heatmap_gp;

#define hue 171
#define sat 255

// Define the array of leds
CRGB leds[NUM_LEDS];
byte prevVal[NUM_LEDS];


//DISTANCE LIMITS IN mm
#define MAX_DIST 1500
#define MIN_DIST 200


///////////////////////////////////////////////////////////////////////////////

void displayColor(float distance, float angle, bool startBit, byte quality)
{
  if(quality > 10 && distance > 10){
    
    angle = constrain(angle, 1, 359);
    distance = constrain(distance, MIN_DIST, MAX_DIST);

    byte val = map(distance, MIN_DIST, MAX_DIST, 255,0);
    byte led = map(angle, 1, 359, 0, NUM_LEDS - 1);
   // byte hue3 = map(distance, 0, MAX_DIST, 0, 100);

    prevVal[led]=(val+prevVal[led])/2;
    
    leds[led] = ColorFromPalette( myPal, prevVal[led]); // normal palette access
   // leds[led] = CHSV( hue3, sat, prevVal[led]);
 

  } 
//  else if (quality <= 10){
//    byte led = map(angle, 1, 359, 0, NUM_LEDS - 1);
//    leds[led] = CHSV( 0, sat, 20);
//  }

  if(startBit){
    FastLED.show();
    
    FastLED.clear();    
  }
  
}

void setup() {
  Serial.begin(9600);

  //LIDAR
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  analogWrite(RPLIDAR_MOTOR, 150);
  //LED
  LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}


void loop() {


  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here...
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

    

    displayColor(distance,angle,startBit,quality);
    

   
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
