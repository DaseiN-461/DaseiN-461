#define sys_freq 16e6




void T1_isr_init(int freq1){

  int top = (sys_freq/(2*1024*freq1)) - 1;
  
  
  //CTC mode
  //TCCR1A = 0B00000000;
  //TCCR1B = 0B00001101;

  //TCCR1A =   0B00000000;
  TCCR1B &=~(1<<WGM13);
  TCCR1B |= (1<<WGM12);
  TCCR1A &=~(1<<WGM11);
  TCCR1A &=~(1<<WGM10); 
  
  //prescaler
  TCCR1B |= (1<<CS12);
  TCCR1B &=~(1<<CS11);
  TCCR1B |= (1<<CS10);
  
  OCR1A = top;

  TIMSK1 = 0B00000010;   

  sei();
}
