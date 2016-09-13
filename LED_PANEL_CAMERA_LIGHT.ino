#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1800

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 

	LEDS.addLeds<WS2812B,5,GRB>(leds,NUM_LEDS);
	LEDS.setBrightness(200);
}

void loop() { 

	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CRGB::White;
		// Show the leds

		// now that we've shown the leds, reset the i'th led to black

	}

    FastLED.show(); 

}
//LightSkyBlue
