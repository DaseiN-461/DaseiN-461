#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
  


TFT_eSPI tft = TFT_eSPI();  

void setup_screen(){
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.println("Wireless - Spending machine");
}
