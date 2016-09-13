//LIDAR Related

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
const byte RPLIDAR_SPEED = 255;

////////////////////////////////////////////////////////////////////////////////
//LED Related
#include <FastLED.h>

#define LED_PIN  5

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);

#define sat 255
#define hue 171

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 60;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);
byte prevVal[NUM_LEDS];


#define MAX_DIST 3500
#define MIN_DIST 200
#define xySize 60

///////////////////////////////////////////////////////////////////////////////

//global matrix XY
byte globalXY[xySize+1][xySize+1];


// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

const byte ledsXcorr = 45;  // Correction in X-direction to center the image
const byte ledsYcorr = 0;   // Correction in Y-direction to center the image

const byte lvl1 = 160;
const byte lvl2 = 96;
const byte lvl3 = 64;
const float pixelFade = 0.8; //used to fade the pixels from last write;

int writeAngle = 0;
bool writeReady = false;
int goodReadCount = 0;
int goodReadCountLimit = 200;
int qualityLimit = 10;

void makeCircle(int xin, int yin){

    //Level 2 write
    for( int x=-1; x<2; x++){
      for(int y=-1; y<2; y++){
        int xcord = xin+x;
        int ycord = yin+y;
        if((globalXY[xin+x][yin+y] < lvl2)&&(xcord>=0)&&(ycord>=0)&&(xcord<xySize)&&(ycord<xySize)){
          globalXY[xin+x][yin+y] = lvl2;
        }
      }
    }

     //Level 3 write
    for( int x=-2; x<3; x++){
      for(int y=-2; y<3; y++){
        int xcord = xin+x;
        int ycord = yin+y;  
        if((globalXY[xin+x][yin+y] < lvl3)&&(xcord>=0)&&(ycord>=0)&&(xcord<xySize)&&(ycord<xySize)){
          globalXY[xin+x][yin+y] = lvl3;
        }
      }
    }
}


void mapXY(float distance, float angle){
//    int xin = (xySize/2*distance/MAX_DIST)*cos((angle+270)*3.14/180)+xySize/2; // old form
//    int yin = (xySize/2*distance/MAX_DIST)*sin((angle+270)*3.14/180)+xySize/2; // old form
    
    int xin = xySize/2 * (distance/MAX_DIST * cos((angle+270)*3.14/180) + 1);
    int yin = xySize/2 * (distance/MAX_DIST * sin((angle+270)*3.14/180) + 1);
    

    //test if input is within array bounds
    if((xin > 60 || xin < 0 || yin > 60 || yin < 0)){
          globalXY[30][30] = lvl3; // Error message: single dim pixel in ceter
    } else {
        globalXY[xin][yin]=lvl1;
       makeCircle(xin,yin);
    }
    
    
}


int XY( int x, int y)
{
  int i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      int reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}


int XYsafe( int x, int y)
{
  if ( (x >= kMatrixWidth)||(x<0)){
    return -1;
  }
  if ( (y >= kMatrixHeight)||(y<0)){
    return -1;
  }
  return XY(x, y);
}

void displayColor(float distance, float angle, bool startBit, byte quality)
{
  
  if(quality > qualityLimit && distance < MAX_DIST && distance > MIN_DIST){
    goodReadCount++;
    mapXY(distance,angle);
  }
  if(startBit){
    writeReady = true;
  }
  if(writeReady && angle>writeAngle && goodReadCount > goodReadCountLimit){
    for(int xin=0; xin<xySize; xin++){
      for(int yin=0; yin<xySize; yin++){
          byte val=globalXY[xin][yin];
          int led = XYsafe(ledsXcorr-xin, ledsYcorr+yin);
          leds[led] = CHSV( (val+prevVal[led])/2, sat, (val+prevVal[led])/2); // Note: X axis flipped with a minus sign.
          globalXY[xin][yin] = val*pixelFade; //gradually erase data from the matrix
          prevVal[led]=(val+prevVal[led])/2;
      }
    }
    FastLED.show();
    writeReady = false;
    writeAngle = (writeAngle + 100) % 360;
    goodReadCount = 0;

    //FastLED.clear();    //doesn't do anything since all LED positions are updated each round anyway. 
  }
  
}

//test code for coordinates
//float angle = 155;
//float distance = 500;
//byte quality = 200;
//bool startBit = true;


void loop() {
  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here...
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

    
    displayColor(distance,angle,startBit,quality);
//  //  delay(10);
//    angle=angle+10;
    
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      //detected...
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, RPLIDAR_SPEED);
      delay(1000);
    }
  }
}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);

  
  //LIDAR
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  analogWrite(RPLIDAR_MOTOR, RPLIDAR_SPEED);
}
