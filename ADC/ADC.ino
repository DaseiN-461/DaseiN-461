#include "ADC.h"

void ADC_init();
uint16_t ADC_read(uint8_t canal);

char str[10];
uint16_t dato_adc;

void setup(){
  Serial.begin(9600);
  ADC_init();
}

void loop() {
  dato_adc = ADC_read(1);
  itoa(dato_adc, str, 10); //conversion de entero a str
  Serial.println(str);
  delay(100);
}
