#include "SPI.h"

char string[30] = "hello world\n";


void setup() {
  SPI_Master_init();
  delay(5000);
  Serial.begin(9600);
}

void loop() {
  SPI_SlaveON(1);
  SPI_TX_string(string);
  SPI_SlaveOFF(1);
  delay(3000);

}
