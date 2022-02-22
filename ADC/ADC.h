void ADC_init(){
  //set voltage reference
  ADMUX |= (1<<REFS0);
  ADMUX &=~ (1<<REFS1);

  //left adjust result
  ADMUX &=~ (1<<ADLAR);

  //set prescaler to 128
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  //ADC enable 
  ADCSRA |= (1<<ADEN);  
}

uint16_t ADC_read(uint8_t canal){
  //evita posibles errores y modificar los registros no deseados
  canal &= 0b00001111;
  ADMUX |= (ADMUX & 0xF0)|canal; 

  //comienza la conversión
  ADCSRA |= (1<<ADSC); 

  //hasta que se complete la conversión
  while((ADCSRA)&(1<<ADSC)); 
  
  return(ADC);
}
