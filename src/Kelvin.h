/*
 Using a potentiometer to selectl kelvin .

 struct   CHSV
  Representation of an HSV pixel (hue, saturation, value (aka brightness))
 
 struct    CRGB
  Representation of an RGB pixel (Red, Green, Blue) 
 */

#include <Arduino.h>
#include <FastLED.h>


#define LED_PIN 13             // which pin are LEDS connected to?
#define NUM_LEDS 7
#define COLOR_ORDER BRG // RGB - BRG
#define LED_TYPE WS2812B        // i'm using WS2811s, FastLED supports lots of different types.
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FRAMES_PER_SECOND 60
#define COOLING 55
#define SPARKING 120

#define MAX_BRIGHTNESS 255      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 0        // set to a minimum of 25%

const int kelvinPin = A0;       // Analog input pin that the speed control potentiometer is attached to.
const int brightnessInPin = A1;

struct CRGB leds[NUM_LEDS];


CRGB kelvinrgbhex[] = {0xFF3800, 0xFF4700, 0xFF5300, 0xFF5D00, 0xFF6500,
                       0xFF6D00, 0xFF7300, 0xFF7900, 0xFF7E00, 0xFF8300,
                       0xFF8A12, 0xFF8E21, 0xFF932C, 0xFF9836, 0xFF9D3F,
                       0xFFA148, 0xFFA54F, 0xFFA957, 0xFFAD5E, 0xFFB165,
                       0xFFB46B, 0xFFB872, 0xFFBB78, 0xFFBE7E, 0xFFC184,
                       0xFFC489, 0xFFC78F, 0xFFC994, 0xFFCC99, 0xFFCE9F,
                       0xFFD1A3, 0xFFD3A8, 0xFFD5AD, 0xFFD7B1, 0xFFD9B6,
                       0xFFDBBA, 0xFFDDBE, 0xFFDFC2, 0xFFE1C6, 0xFFE3CA,
                       0xFFE4CE, 0xFFE6D2, 0xFFE8D5, 0xFFE9D9, 0xFFEBDC,
                       0xFFECE0, 0xFFEEE3, 0xFFEFE6, 0xFFF0E9, 0xFFF2EC,
                       0xFFF3EF, 0xFFF4F2, 0xFFF5F5, 0xFFF6F7, 0xFFF8FB,
                       0xFFF9FD, 0xFEF9FF, 0xFCF7FF, 0xF9F6FF, 0xF7F5FF,
                       0xF5F3FF, 0xF3F2FF, 0xF0F1FF, 0xEFF0FF, 0xEDEFFF,
                       0xEBEEFF, 0xE9EDFF, 0xE7ECFF, 0xE6EBFF, 0xE4EAFF,
                       0xE3E9FF, 0xE1E8FF, 0xE0E7FF, 0xDEE6FF, 0xDDE6FF,
                       0xDCE5FF, 0xDAE5FF, 0xD9E3FF, 0xD8E3FF, 0xD7E2FF,
                       0xD6E1FF, 0xD4E1FF, 0xD3E0FF, 0xD2DFFF, 0xD1DFFF,
                       0xD0DEFF, 0xCFDDFF, 0xCFDDFF, 0xCEDCFF, 0xCDDCFF,
                       0xCFDAFF, 0xCFDAFF, 0xCED9FF, 0xCDD9FF, 0xCCD8FF,
                       0xCCD8FF, 0xCBD7FF, 0xCAD7FF, 0xCAD6FF, 0xC9D6FF,
                       0xC8D5FF, 0xC8D5FF, 0xC7D4FF, 0xC6D4FF, 0xC6D4FF,
                       0xC5D3FF, 0xC5D3FF, 0xC5D2FF, 0xC4D2FF, 0xC3D2FF,
                       0xC3D1FF};



int kelvincolor[] = {1000, 1100, 1200, 1300, 1400,
                     1500, 1600, 1700, 1800, 1900,
                     2000, 2100, 2200, 2300, 2400,
                     2500, 2600, 2700, 2800, 2900,
                     3000, 3100, 3200, 3300, 3400,
                     3500, 3600, 3700, 3800, 3900,
                     4000, 4100, 4200, 4300, 4400,
                     4500, 4600, 4700, 4800, 4900,
                     5000, 5100, 5200, 5300, 5400,
                     5500, 5600, 5700, 5800, 5900,
                     6000, 6100, 6200, 6300, 6400,
                     6500, 6600, 6700, 6800, 6900,
                     7000, 7100, 7200, 7300, 7400,
                     7500, 7600, 7700, 7800, 7900,
                     8000, 8100, 8200, 8300, 8400,
                     8500, 8600, 8700, 8800, 8900,
                     9000, 9100, 9200, 9300, 9400,
                     9500, 9600, 9700, 9800, 9900,
                     10000, 10100, 10200, 10300, 10400,
                     10500, 10600, 10700, 10800, 10900,
                     11000, 11100, 11200, 11300, 11400,
                     11500, 11600, 11700, 11800, 11900,
                     12000};


// void setup() {
//   delay(3000); // in case we do something stupid. We dont want to get locked out.

//   LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
//   FastLED.setBrightness(MAX_BRIGHTNESS);
// }

void Kelvin() {
  // read the analog brightness value:
  //int brightValue = analogRead(redPin);            
  // map it to the range of the FastLED brightness:
  int kelvin = map(analogRead(kelvinPin), 0, 1023, 0, 112);
  int brightValue = map(analogRead(brightnessInPin), 0, 1023, 0, MAX_BRIGHTNESS);

 // FastLED.setBrightness(constrain(brightValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
  FastLED.setBrightness(brightValue);
  
 // for(int n = 0; n < NUM_LEDS; n++)
 // {
 //  leds[n] = CRGB(redValue, greenValue, blueValue);
 // }
  fill_solid(leds, NUM_LEDS, CRGB(kelvinrgbhex[kelvin]));
  // display kelvincolor[kelvin] 
  FastLED.show();



}