#include "effects.h"

uint8_t effect;
bool stop = false;
uint8_t dotHue = 220;
uint8_t temp;

GRGB led;

#define KELVIN 0
#define COLORS 1
#define LIGHTER 2
#define RAINBOW_UP 3
#define FIRE 4

void ShowEffect(){
  static uint32_t _speed = 50; // чем больше, тем медленнее
  static uint32_t last_time = 0;

  // костыли
  if(effect == FIRE) {
    _speed = 20;
  } else {
    _speed = 50;
  }

  if (millis() - last_time >= _speed && !stop)
  {
    last_time = millis();

    Serial.println(effect); 

    switch(effect){
      case KELVIN: 
        SetKelvin(temp);
        break;
      case COLORS: Colors();
        FastLED.show();
        break;
      case LIGHTER: Lighter(dotHue);
        FastLED.show();
        break;
      case RAINBOW_UP: RainbowUp();
        FastLED.show();
        break;
      case FIRE: Fire();
        break;
      default: effect = 0;
    }
  }
}


void RainbowDown(){
  static uint8_t color = 0;
  byte hue = 255 / (RAINBOW_LENGHT * NUM_LEDS);

  color += 2;
  for (byte i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(color + i * hue, 255, 255);
}

void RainbowUp(){
  static uint8_t color = 0;
  byte hue = 255 / (RAINBOW_LENGHT*NUM_LEDS);

  color += 2;
  for (byte i = 0; i < NUM_LEDS; i++)
    leds[NUM_LEDS - 1 - i] = CHSV(color + i * hue, 255, 255);
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

void Fade(uint8_t speed) { // обновляет всем цветам затухание
  for (int i = 0; i < NUM_LEDS; i++) {
    if (getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(speed);
  }
}

void Bloom(uint8_t speed){
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeLightBy(speed);
  }
}

void Sparkles() { // конфети
  byte thisNum = random(NUM_LEDS/2, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  Fade();
}

void FillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}

// ****************************** ОГОНЁК ******************************

void Lighter(uint8_t newcolor) {
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

  leds[position] = CHSV(newcolor, 255, 255);
}

void Colors(){
  static byte hue;
  hue ++;
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
    FillAll(CHSV(hue, 255, 255));
  }
}

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

void SetKelvin(byte kelvin){ // 0 - 112
  kelvin = constrain(kelvin, 0, 112);
  fill_solid(leds, NUM_LEDS, CRGB(kelvinrgbhex[kelvin]));
  // display kelvincolor[kelvin] 
  FastLED.show();
}


struct Data {
  bool state = 1;     // 0 выкл, 1 вкл
  byte mode = 0;      // 0 цвет, 1 теплота, 2 огонь
  byte bright[3] = {30, 30, 30};  // яркость
  byte value[3] = {0, 0, 0};      // параметр эффекта (цвет...)
};

Data data;

// огненный эффект
void Fire() {
  static uint32_t rnd_tmr, move_tmr;
  static int rnd_val, fil_val;
  FillAll(CHSV(3, 255, 255)); // для выключения через красноватый цвет
  
  // таймер 100мс, генерирует случайные значения
  if (millis() - rnd_tmr > 100) {
    rnd_tmr = millis();
    rnd_val = random(0, 13);
  }
  
  // таймер 20мс, плавно движется к rnd_val
  if (millis() - move_tmr > 20) {
    move_tmr = millis();
    // эксп фильтр, на выходе получится число 0..120
    fil_val += (rnd_val * 10 - fil_val) / 5;

    // преобразуем в яркость от 100 до 255
    int br = map(fil_val, 0, 120, 100, 255);

    // преобразуем в цвет как текущий цвет + (0.. 24)
    int hue = data.value[2] + fil_val / 5;
    led.setWheel8(hue, br);
  }
}