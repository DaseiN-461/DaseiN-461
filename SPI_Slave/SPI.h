

void SPI_Slave_init(void){
  /* Set MISO output, all others input */
  DDRB &=~ (1<<DDB2); // SS PIN 10 Arduino
  DDRB &=~ (1<<DDB3); // MOSI PIN 11 Arduino
  DDRB |= (1<<DDB4); // MISO PIN 12 Arduino
  DDRB &=~ (1<<1<<DDB5); // SCK PIN 13 Arduino

  /* Set as Slave */
  SPCR &=~ (1<<MSTR);
  
  /* Enable SPI */
  SPCR |= (1<<SPE);
}
char SPI_SlaveReceive(void){
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  
  /* Return Data Register */
  return SPDR;
}

String get_line_SPI(){
  String string = "";
  while(1){
    char c = SPI_SlaveReceive();
    if(c == '\n'){
      return string;
    }else{
      string += c;
    }
  }
}
