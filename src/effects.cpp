#include "effects.h"

void RainbowDown(){
  static uint8_t color = 0;
  byte hue = 255 / (RAINBOW_LENGHT * NUM_LEDS);

  color += 2;
  for (byte i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(color + i * hue, 255, 255);
}

void RainbowUp(){
  static uint8_t color = 0;
  byte hue = 255 / (2*NUM_LEDS);

  color += 2;
  for (byte i = 0; i < NUM_LEDS; i++)
    leds[NUM_LEDS - 1 - i] = CHSV(color + i * hue, 255, 255);
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

#define FADE_SPEED 10 // скорость угасания, чем больше тем быстрее

void fade() { // обновляет всем цветам затухание
  for (int i = 0; i < NUM_LEDS; i++) {
    if (getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(FADE_SPEED);
  }
}

void Sparkles() { // конфети
  byte thisNum = random(0, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  fade();
}

void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}

// ****************************** ОГОНЁК ******************************

void Lighter(byte r, byte g, byte b) {
  static uint16_t position;
  static boolean direction = true;

  FastLED.clear();

  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }

  leds[position].setRGB(r, g, b);
}

void Colors(){
  static byte hue;
  hue ++;
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
    fillAll(CHSV(hue, 255, 255));
  }
}