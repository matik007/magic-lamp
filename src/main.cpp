#include <Arduino.h>
#include <FastLED.h>

#include "effects.h"

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // очистим ленту
  FastLED.clear();
  FastLED.show(); 
  delay(1);     // между вызовами show должна быть пауза минимум 40 мкс !!!!
}

void loop() {
  static uint32_t _speed = 50; // чем больше, тем медленнее
  static uint32_t last_time = 0;

  if (millis() - last_time >= _speed)
  {
    last_time = millis();
    Lighter(255, 255, 0);
    FastLED.show();
  }
}

