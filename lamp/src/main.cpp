#include <Adafruit_NeoPixel.h>

//Адрес пина
#define LED_PIN    5


//Количество светодиодов в ленте
#define LED_COUNT 10

//Яркость
#define brightnes 255

//Создание экземпляра Adafruit_NeoPixel класса с адресом и количеством светодиодов
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);




void setup() {

  strip.begin();           //Инициализация объекта
  strip.show();            
  strip.setBrightness(brightnes); // Установка яркости
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // Установить один цвет для всей ленты
    strip.setPixelColor(i, color);         // 
    strip.show();                          //  
    delay(wait);                           //
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {
    for(int b=0; b<3; b++) {
      strip.clear();         
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); 
      }
      strip.show();
      delay(wait); 
    }
  }
}


void rainbow(int wait) {

  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}

void loop() {
  // Один цвет вдоль всей ленты
  colorWipe(strip.Color(127, 127, 127), brightnes);
//  colorWipe(strip.Color(255,   0,   0), brightnes); // Red
  // colorWipe(strip.Color(  0, 255,   0), brightnes0); // Green
//   colorWipe(strip.Color(  0,   0, 255), brightnes); // Blue

//   Мерцание разных цветов
//   theaterChase(strip.Color(127, 127, 127), brightnes); // Белый, половина яркости
//   theaterChase(strip.Color(127,   0,   0), brightnes); // Красный
//    theaterChase(strip.Color(  0,   0, 127), brightnes); // Голубой

  //Радуга с заданной задержкой
//  rainbow(50);
}