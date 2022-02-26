#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

const int n_samples = 140;
const int sample_time = 10;


int data[n_samples] = {};

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);

  Serial.println();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  
}

void refresh_screen(){
  tft.fillRect(61,20,152,99,TFT_BLACK);
  tft.drawLine(60,120,60,20,TFT_WHITE);
  tft.drawLine(60,120,210,120,TFT_WHITE);

  tft.setCursor(120,122);
  tft.print("time");

  tft.setCursor(35,20);
  tft.print("4096");

  tft.setCursor(50,115);
  tft.print("0");
  
  tft.setCursor(110,5);
  tft.print("ADC plot");

}

double promedio(int arreglo[], int cantidad){
  double suma = 0;
  for(int i=0; i<cantidad-1; i++){
    suma = suma + arreglo[i];
  }
  return suma/cantidad;
}

void loop() {
  refresh_screen();

  tft.setCursor(10,50);
  tft.print("average");
  tft.setCursor(10,60);
  tft.print(promedio(data,n_samples));

  tft.setCursor(10,70);
  tft.print("freq");

  tft.setCursor(10,80);
  tft.print("100 Hz");
  
  int xpos = 70;
  int ypos = 120;

  int sample = analogRead(2);
  data[n_samples-1] = sample;

  for(int i=0; i<n_samples; i++){  //barre con i de 0 a 9
    data[i] = data[i+1];
  }

  

  for(int i=0; i<n_samples-1; i++){ //barre con i de 0 a 8
    int val = map(data[i],0, 4095, 0, 100);

    int xpos_now = (xpos + i);
    int ypos_now = ypos -  val;
    tft.drawPixel(xpos_now,ypos_now,TFT_WHITE);
  }
  

  delay(sample_time);

}

