#include <avr/io.h>
inline void nop(void){
  asm volatile("nop");
}
void send_byte(uint8_t byte){
  PORTB |= _BV(1);
  nop();
  nop();
  PORTB &= ~_BV(1);
  for(int i=0;i<8;i++){
    if(byte & 0x80){
      asm volatile("sbi 0x05, 0\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "cbi 0x05, 0\n\t"
		   "nop\n\t"
		   "nop \n\t"
		   );
    }else{
      asm volatile("sbi 0x05, 0\n\t"
		   "nop\n\t"
		   "cbi 0x05, 0\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "nop\n\t"
		   "nop\n\t"
);

    }
    byte <<= 1;
  }
}
