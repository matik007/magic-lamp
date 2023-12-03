/*
* - Одиночное касание - вкл/выкл
* - Двойное касание - следующий режим
* - Удержание - остановить/продложить
*/


#include <Arduino.h>
#include <FastLED.h>
#include "GyverButton.h"

#include "effects.h"

#define BTN_PIN 2

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

CRGB leds[NUM_LEDS];

uint8_t state;
uint8_t lastState;
uint8_t effect;

bool powerOn;
bool lastPower = true;
bool stop = false;

#define OFF 0

void ShowEffect();

void setup() {
  Serial.begin(9600);

  touch.setTimeout(300);
  touch.setClickTimeout(500);

  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // очистим ленту
  FastLED.clear();
  FastLED.show(); 
  delay(1);     // между вызовами show должна быть пауза минимум 40 мкс !!!!
}

void loop() {
  touch.tick();

  if(touch.isSingle()){
    powerOn = !powerOn;
  }
  if(touch.isDouble()){
    effect++;
  }
  if(touch.isHolded() && powerOn){
    stop = !stop;
  }

  if(powerOn){
    if(lastPower != powerOn){
      lastPower = powerOn;
      stop = false;
      Bloom();
    }
    ShowEffect();
  } else {
    stop = true;
    lastPower = powerOn;
    Fade(5);
    FastLED.show();
  }
}

void ShowEffect(){
  static uint32_t _speed = 50; // чем больше, тем медленнее
  static uint32_t last_time = 0;

  if (millis() - last_time >= _speed && !stop)
  {
    last_time = millis();

    Serial.println(effect);

    switch(effect){
      case 0: Lighter(255, 0, 255);
        break;
      case 1: Colors();
        break;
      case 2: RainbowUp();
        break;
      case 3: RainbowDown();
        break;
      case 4: Sparkles();
        break;
      default: effect = 0;
    }
    FastLED.show();
  }
}