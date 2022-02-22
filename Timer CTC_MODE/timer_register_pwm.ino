#include "isr.h"

bool state = false;



void setup(){
  T1_isr_init(1);
  Serial.begin(9600);

  DDRB = (1<<PB5);
  PORTB = (1<<PB5);
}

void loop(){
 
}


ISR(TIMER1_COMPA_vect){
  state = !state;
  PORTB = (state<<PB5);
}
