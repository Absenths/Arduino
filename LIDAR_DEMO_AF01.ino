#include <RPLidar.h>

#include <FastLED.h>

#define LED_PIN  5

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 64

const int Ain = A2;

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);


// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 60;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
// Set 'kMatrixSerpentineLayout' to false if your pixels are
// laid out all running the same way, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19
//
// Set 'kMatrixSerpentineLayout' to true if your pixels are
// laid out back-and-forth, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15
//
// Bonus vocabulary word: anything that goes one way
// in one row, and then backwards in the next row, and so on
// is call "boustrophedon", meaning "as the ox plows."


// This function will return the right 'led index number' for
// a given set of X and Y coordinates on your matrix.
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < kMatrixWidth; x++) {
//      for( uint8_t y = 0; y < kMatrixHeight; y++) {
//
//        // Here's the x, y to 'led index' in action:
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//
//      }
//    }
//
//
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}


#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

//uint16_t XYsafe( int x, int y)
//{
//  if((x<0)||(y<0)) return -1;
//  uint8_t ux = x;
//  uint8_t uy = y;
//  if( ux >= kMatrixWidth) return -1;
//  if( uy >= kMatrixHeight) return -1;
//  return XY(ux,uy);
//}

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if ( x >= kMatrixWidth) return -1;
  if ( y >= kMatrixHeight) return -1;
  return XY(x, y);
}

void showLidar() {
  FastLED.show();
  delay(random(30, 50));
  FastLED.clear();
}

void makeRing(byte xp, byte yp) {

  for ( byte y = yp - 1; y < yp + 2; y++) {
    leds[ XYsafe(xp - 1, y)] = CHSV(85, 255, 255);
    leds[ XYsafe(xp + 1, y)] = CHSV(85, 255, 255);
  }
  leds[ XYsafe(xp, yp - 1)] = CHSV(85, 255, 255);
  leds[ XYsafe(xp, yp + 1)] = CHSV(85, 255, 255);

}
bool bol =true;
void loop() {

while(bol){
  int sens = analogRead(Ain)/15;
  if(sens<14){
  
    leds[ XYsafe(sens+15, 1)] = CHSV(0, 255, 255);

    for ( int y = -1; y < 2; y++) {
      leds[ XYsafe(sens+15 - 1, 1 + y)] = CHSV(85, 255, 255);
      leds[ XYsafe(sens+15 + 1, 1 + y)] = CHSV(85, 255, 255);
    }
    leds[ XYsafe(sens+15, 0)] = CHSV(85, 255, 255);
    leds[ XYsafe(sens+15, 2)] = CHSV(85, 255, 255);

    
    showLidar();
  }else{

    leds[ XYsafe(28, sens-13)] = CHSV(0, 255, 255);

    for ( int x = -1; x < 2; x++) {
      leds[ XYsafe(28 + x, sens -13 - 1)] = CHSV(85, 255, 255);
      leds[ XYsafe(28 + x, sens -13 + 1)] = CHSV(85, 255, 255);
    }
    leds[ XYsafe(27, sens -13)] = CHSV(85, 255, 255);
    leds[ XYsafe(29, sens - 13)] = CHSV(85, 255, 255);

    showLidar();
    
  }


  //bol=false;
}
}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}




