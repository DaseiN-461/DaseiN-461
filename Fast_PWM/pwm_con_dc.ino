#include "PWM.h"

String header;
void setup(){
  Serial.begin(9600);
  cli();
  PWM_init(100);
  sei();

  PWM_setDutyA(5);
  PWM_setDutyB(30);

  PWM_on();
  
  
}
void fade(){
  for(int i=1; i<70; i++){
    PWM_setDutyA(i);
    delay(10);
  }
}

void loop(){
  fade();
  
}
