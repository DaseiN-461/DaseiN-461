#include "SPI.h"

void setup() {
  SPI_Slave_init();
  Serial.begin(9600);
}

void loop() {
  String string = get_line_SPI();
  Serial.println(string);
}
