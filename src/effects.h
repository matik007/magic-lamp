#pragma once

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 13     // пин ленты
#define NUM_LEDS 7      // кол-во светодиодов

#define FADE_SPEED 10 // скорость угасания, чем больше тем быстрее

#define BRIGHTNESS 255

#define RAINBOW_LENGHT 3 // длинна радуги (1 - радуна провторяется каждые NUM_LEDS светодиодов, и сливается в маленьком светильнике в белый)

extern CRGB leds[NUM_LEDS];
extern uint8_t effect;
extern uint8_t dotHue; // цвет режима огонёк
extern bool stop;

void FillAll(CRGB newcolor);
void Fade(uint8_t speed = FADE_SPEED);
void Bloom(uint8_t speed = FADE_SPEED);
void Colors();
void RainbowDown();
void RainbowUp();
void Sparkles();
void Lighter(uint8_t newcolor);
void ShowEffect();