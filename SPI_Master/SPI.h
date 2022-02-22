void SPI_Master_init(void){
  /* Set MOSI and SCK output */
  DDRB |= (1<<DDB3)|(1<<DDB5);
  /* Set MISO as input */
  DDRB &=~ (1<<DDB4);
  /* Set SS as output */
  DDRB |= (1<<DDB2);
  /* Set SS state to high */
  //Slave can't communicate to master
  PORTB &=~ (1<<PORTB2);
  /* Sequence of out bits  
            - DORD = 0, MSB first        - DORD = 1, LSB first*/
  SPCR &=~ (1<<DORD);
  /* Polarity and phase config */
  SPCR &=~ (1<<CPOL);
  SPCR &=~ (1<<CPHA);
  /* Enable SPI, Master, set clock rate fck/16 (f_spi = 1MHz) */
  SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_SlaveON(uint8_t slave){
  switch(slave){
    case 1:
      PORTB &=~ (1<<PORTB2);
      break;
  }
}

void SPI_SlaveOFF(uint8_t slave){
  switch(slave){
    case 1:
      PORTB |= (1<<PORTB2);
      break;
  }
}

void SPI_TX(char data){
  /* Start transmission */
  SPDR = data;
  
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
}

uint8_t SPI_RX(){
  /* wait for the data */
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}

void SPI_TX_string(char string[30]){
  for(int i=0; i<30; i++){
    char c = string[i]; 
    if(c == '\n'){
      SPI_TX('\n');
      return 0;
    }else{
      SPI_TX(c);
      delay(10);
    }
  }
}
