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
#define NUM_LEDS 53
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


///////////////////////////////////////////////////////////////////////////////

void displayColor(float distance, float angle, byte quality)
{

  angle = constrain(angle, 1, 359);
  distance = constrain(distance, 0, MAX_DIST);

  byte val = map(distance, 0, MAX_DIST, 255,10);
  byte led = map(angle, 1, 359, 0, NUM_LEDS - 1);

  leds[led] = CHSV( hue, sat, val);

  FastLED.show();
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


void loop() {


  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here...
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

 
    if(quality > 10 && distance > 10){
    displayColor(distance,angle,quality);
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
