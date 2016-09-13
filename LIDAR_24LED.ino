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
#define NUM_LEDS 1

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

//Set current RGB with the hue: HSV(hue, x, x)
void hue_to_rgb( _u8 hue)
{
/*`
    Hue(in Degree):  0 (RED) ----> 60 (Yello) ----> 120 (Green) --...... ----> 360
    Hue'(fit to _u8):0       ----> 60/360*255 ----> 120/260*255 --...... ----> 255
*/
    
    //convert HSV(hue,1,1) color space to RGB space
    if (hue < 120L*255/360)   //R->G
    {
        _g = hue;
        _r = NEO_RGBSPACE_MAX - hue;
        _b = 0;
    }else if (hue < 240L*255/360)  //G->B
    {
        hue -= 120L*255/360;
        _b = hue;
        _g = NEO_RGBSPACE_MAX - hue;
        _r = 0;
    }else //B->R
    {
        hue -= 240L*255/360;
        _r = hue;
        _b = NEO_RGBSPACE_MAX - _r;
        _g = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void displayColor(float angle, float distance)
{
    //1. map 0-350 deg to 0-23
    byte hue = angle*255/360;
    hue_to_rgb(hue);
    
    //2. control the light 
    
    int lightFactor = (distance>500.0)?0:(255-distance*255/500);
    _r *=lightFactor;
    _g *=lightFactor;
    _b *=lightFactor;
    
    _r /= 255;
    _g /= 255;
    _b /= 255;    

    leds[0].r = 255 - _r; 
    leds[0].g = 255 - _g; 
    leds[0].b = 255 - _b;
FastLED.show();
}

void setup() {
  Serial.begin(9600);
  
  //LIDAR
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);

  //LED
    LEDS.addLeds<CHIPSET,LED_PIN,RGB>(leds,NUM_LEDS);
}

float minDistance = 100000;
float angleAtMinDist = 0;

void loop() {
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
