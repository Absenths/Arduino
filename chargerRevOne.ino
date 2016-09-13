//LED Related
#include <FastLED.h>

#define LED_PIN  5

#define COLOR_ORDER RBG
#define CHIPSET     WS2811

#define BRIGHTNESS 64

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);

// Params for width and height
const uint8_t kMatrixWidth = 5;
const uint8_t kMatrixHeight = 4;
const bool    kMatrixSerpentineLayout = true;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);
// byte prevVal[NUM_LEDS];

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
  if ( (x >= kMatrixWidth) || (x < 0)) {
    return -1;
  }
  if ( (y >= kMatrixHeight) || (y < 0)) {
    return -1;
  }
  return XY(x, y);
}

byte waveHue = 170;
// green = 96;
// blue = 160;
// purple = 192



void chargeWave() {
  for (int x = 0; x < 5; x++) {
    for (int val = 0; val < 256; val++) {
      leds[XYsafe(x, 0)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 1)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 2)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 3)] = CHSV(waveHue, 255, val);
      FastLED.show();
      delay(1);
    }
    FastLED.show();
  }

  for (int val = 255; val >= 0; val--) {
    for (int x = 0; x < 5; x++) {
      leds[XYsafe(x, 0)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 1)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 2)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 3)] = CHSV(waveHue, 255, val);
    }
    FastLED.show();
  }
}


void breathe() {
      for (int val = 90; val < 256; val++) {
  for (int x = 0; x < 5; x++) {

      leds[XYsafe(x, 0)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 1)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 2)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 3)] = CHSV(waveHue, 255, val);
    }
    FastLED.show();
    delay(6);
  }

  for (int val = 255; val >= 90; val--) {
    for (int x = 0; x < 5; x++) {
      leds[XYsafe(x, 0)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 1)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 2)] = CHSV(waveHue, 255, val);
      leds[XYsafe(x, 3)] = CHSV(waveHue, 255, val);
    }
    FastLED.show();
    delay(6);
  }
}

  


void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  breathe();
  delay(200);
  //chargeWave();
}

