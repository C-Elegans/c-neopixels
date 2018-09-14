
#define F_CPU 8000000
#define PIXELS 60
#define BRIGHTSHIFT 0
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay_basic.h>
#include <util/delay.h>

void send_byte(uint8_t);
 
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

const uint8_t PROGMEM delays[] = {
  10,9,6,6,4,4,4,3,3,3,3,3,4,4,4,6,6,9,10,10,0
};

const uint8_t lut[] = {
  0x40, 0x80, 0xff, 0xff, 0xff, 0x80, 0x40, 0x40
};

typedef struct pixel{
  uint8_t g;
  uint8_t r;
  uint8_t b;
} pixel;
pixel pixels[PIXELS];

 
void wheel(uint8_t pos, pixel* pixel){
  pos = 255-pos;
  if(pos<85){
    pixel->r = 255-pos*3;
    pixel->g = 0;
    pixel->b = pos*3;
    return;
  }
  if(pos < 170) {
    pos -= 85;
    pixel->r=0;
    pixel->g=pos*3;
    pixel->b=255-pos*3;
    return;
  }
  pos -=170;
  pixel->r=pos*3;
  pixel->g=255-pos*3;
  pixel->b=0;
}



int main(void){
  DDRB |= _BV(0) | _BV(1);
  PORTB &= ~_BV(0);
  volatile int8_t pos = 40;
  volatile int8_t flag = 0;
  while(1){
    for(int i=0;i<PIXELS;i++){
      uint8_t color = 0;
      if(i-pos/2 <4 && i-pos/2 >= 0){
	color = lut[i*2-pos+1];
      }
      else if(i<42 && i>=19)
	color = 0x08;
      send_byte(0);
      send_byte(color);
      send_byte(0);
    }
    if(flag == 0)
      pos++;
    else
      pos--;
    if(pos >= 2*38){
      flag = 1;
    }
    if(pos < 40)
      flag = 0;

    _delay_loop_1(0xff);
    uint8_t delay = pgm_read_byte(&delays[pos/2-20]);
    for(int i=0;i<delay;i++){
      _delay_ms(10);
    }
  }
}
