/*
* - Одиночное касание - вкл/выкл
* - Двойное касание - следующий режим
* - Удержание - остановить/продложить
*/


#include <Arduino.h>
#include <FastLED.h>
#include "GyverButton.h"
#include "VirtualButton.h"

#include "effects.h"

#define BTN_PIN 2
#define ECHO_PIN 4
#define TRIG_PIN 3

VButton gest;

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

CRGB leds[NUM_LEDS];

uint8_t brightness = BRIGHTNESS;

uint8_t state;
uint8_t lastState;

bool powerOn = false;
bool lastPower = true;



#define OFF 0

int getDist(byte trig, byte echo);
int getFilterMedian(int newVal);
int getFilterSkip(int val);
int getFilterExp(int val);
void pulse();
void DistanceControl();
void TouchControl();


void setLED() {
  FastLED.showColor(CRGB(led.R, led.G, led.B));
}

void setup() {
  Serial.begin(115200);

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);

  touch.setTimeout(300);
  touch.setClickTimeout(500);

  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // очистим ленту
  FastLED.clear();
  FastLED.show(); 
  delay(1);     // между вызовами show должна быть пауза минимум 40 мкс !!!!

  led.attach(setLED);
  led.setCRT(1);
}

void loop() {
  DistanceControl();
  TouchControl();
  
  if(powerOn){
    if(lastPower != powerOn){
      lastPower = powerOn;
      stop = false;
      for(int i = 0; i < brightness; i++){
        FastLED.setBrightness(i);
        FastLED.show();
        delay(2);
      }
    }
    ShowEffect();
  } else {
    if(lastPower != powerOn){
      stop = true;
      lastPower = powerOn;
      // Fade(5);
      for(int i = brightness; i > 0; i--){
        FastLED.setBrightness(i);
        FastLED.show();
        delay(2);
      }
    }
  }
}

void DistanceControl(){
  int dist = getDist(TRIG_PIN, ECHO_PIN); // получаем расстояние
  dist = getFilterMedian(dist);         // медиана
  dist = getFilterSkip(dist);           // пропускающий фильтр
  int dist_f = getFilterExp(dist);      // усреднение
  Serial.println(dist_f);

  gest.poll(dist); // расстояние больше 0 - это клик

  if(gest.hasClicks()){
    switch (gest.clicks) {
      case 1:
        powerOn = !powerOn;  // вкл/выкл
        break;
      case 2:
        if(powerOn) effect++;
        break;
      case 3:
        if(powerOn) effect--;
        break;
    }
    
  }

  // удержание (выполнится однократно)
  if(gest.held() && powerOn){
    switch (gest.clicks) {
      case 0:
        stop = !stop;
      break;
      case 1:
      // тут можно сохранить значение dist и от него регулировать яркость ниже, тогда не будет рыка на увень яркости
      break;
    }
    
  }

   // удержание (выполнится пока удерживается)
    if (gest.hold() && powerOn){
       switch (gest.clicks) {
        case 1:
          brightness = constrain(map(dist, 60, 300, 40, 255), 50, 255);
          FastLED.setBrightness(brightness);
          FastLED.show();
        break;
        case 2: 
          dotHue = constrain(map(dist, 60, 300, 0, 255), 0, 255);
          temp = constrain(map(dist, 60, 300, 0, 112), 0, 120);
          Serial.print("Temp = ");
          Serial.print(temp);
          Serial.print(" || Dist = ");
          Serial.println(dist);
        break;
      }
    }
}

void TouchControl(){
  touch.tick();

  if(touch.isSingle()){
    powerOn = !powerOn;
  }
  if(touch.isDouble()){
    effect++;
    stop = false;
  }
  if(touch.isHolded() && powerOn){
    stop = !stop;
  }
}

int prev_br;

void pulse() {
  for (int i = prev_br; i < prev_br + 45; i += 3) {
    FastLED.setBrightness(min(255, i));
    delay(10);
  }
  for (int i = prev_br + 45; i > prev_br; i -= 3) {
    FastLED.setBrightness(min(255, i));
    delay(10);
  }
}

// получение расстояния с дальномера
#define HC_MAX_LEN 1000L  // макс. расстояние измерения, мм
int getDist(byte trig, byte echo) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // измеряем время ответного импульса
  uint32_t us = pulseIn(echo, HIGH, (HC_MAX_LEN * 2 * 1000 / 343));

  // считаем расстояние и возвращаем
  return (us * 343L / 2000);
 
}

// медианный фильтр
int getFilterMedian(int newVal) {
  static int buf[3];
  static byte count = 0;
  buf[count] = newVal;
  if (++count >= 3) count = 0;
  return (max(buf[0], buf[1]) == max(buf[1], buf[2])) ? max(buf[0], buf[2]) : max(buf[1], min(buf[0], buf[2]));
}

// пропускающий фильтр
#define FS_WINDOW 7   // количество измерений, в течение которого значение не будет меняться
#define FS_DIFF 80    // разница измерений, с которой начинается пропуск
int getFilterSkip(int val) {
  static int prev;
  static byte count;

  if (!prev && val) prev = val;   // предыдущее значение 0, а текущее нет. Обновляем предыдущее
  // позволит фильтру резко срабатывать на появление руки

  // разница больше указанной ИЛИ значение равно 0 (цель пропала)
  if (abs(prev - val) > FS_DIFF || !val) {
    count++;
    // счётчик потенциально неправильных измерений
    if (count > FS_WINDOW) {
      prev = val;
      count = 0;
    } else val = prev;
  } else count = 0;   // сброс счётчика
  prev = val;
  
  return val;
}

// экспоненциальный фильтр со сбросом снизу
#define ES_EXP 2L     // коэффициент плавности (больше - плавнее)
#define ES_MULT 16L   // мультипликатор повышения разрешения фильтра
int getFilterExp(int val) {
  static long filt;
  if (val) filt += (val * ES_MULT - filt) / ES_EXP;
  else filt = 0;  // если значение 0 - фильтр резко сбрасывается в 0
  // в нашем случае - чтобы применить заданную установку и не менять её вниз к нулю
  return filt / ES_MULT;
}