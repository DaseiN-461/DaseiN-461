#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>


#if CONFIG_FREERTOS_UNICORE
  static const  BaseType_t app_cpu = 0;
#else 
  static const BaseType_t app_cpu = 1;
#endif

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;
static TaskHandle_t task_3 = NULL;
static TaskHandle_t task_4 = NULL;

TFT_eSPI tft = TFT_eSPI();

int pos_x_red = 70;
int pos_y_red = 60;

int pos_x_yellow = 70;
int pos_y_yellow = 120;

int pos_x_green = 70;
int pos_y_green = 180;

int radio = 30;

int count = 0;

void red_timer(void *parameter){
  while(1){
      tft.fillCircle(pos_x_red,pos_y_red,radio,TFT_RED);
      vTaskDelay(2000/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_red,pos_y_red,radio,TFT_RED);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_red,pos_y_red,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_red,pos_y_red,radio,TFT_RED);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_red,pos_y_red,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS);

      
  }
}

void yellow_timer(void *parameter){
  while(1){

      tft.fillCircle(pos_x_yellow,pos_y_yellow,radio,TFT_YELLOW);
      vTaskDelay(2000/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_yellow,pos_y_yellow,radio,TFT_YELLOW);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_yellow,pos_y_yellow,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_yellow,pos_y_yellow,radio,TFT_YELLOW);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_yellow,pos_y_yellow,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS);

  }
}


void green_timer(void *parameter){
  while(1){

      tft.fillCircle(pos_x_green,pos_y_green,radio,TFT_GREEN);
      vTaskDelay(2000/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_green,pos_y_green,radio,TFT_GREEN);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_green,pos_y_green,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_green,pos_y_green,radio,TFT_GREEN);
      vTaskDelay(250/portTICK_PERIOD_MS);

      tft.fillCircle(pos_x_green,pos_y_green,radio,TFT_BLACK);
      vTaskDelay(250/portTICK_PERIOD_MS); 

  }
}

void refresh_clock(){
  tft.fillRect(52,2,60,20,TFT_WHITE);
}

void timer(void *parameter){
  while(1){
    
    for(int j=0; j<60; j++){ //minutes
        for(int i=0; i<60; i++){ //seconds
            refresh_clock();
            tft.setCursor(50,5);
            tft.print(j);
            tft.print(":");
            tft.print(i);
            tft.print("  ");
            tft.print(count);
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }
    }
  }
}

void task_scheduler();

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(4);
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(25,25,90,200,TFT_BLACK);
  tft.setTextColor(TFT_BLACK);
  tft.setTextFont(2);

  xTaskCreatePinnedToCore(red_timer,"red", 1024, NULL, 1, &task_1, app_cpu);
  xTaskCreatePinnedToCore(yellow_timer,"yellow", 1024, NULL, 1, &task_2, app_cpu);
  xTaskCreatePinnedToCore(green_timer,"green", 1024, NULL, 1, &task_3, app_cpu);
  xTaskCreatePinnedToCore(timer,"efsfes", 1024, NULL, 1, &task_4, app_cpu);
  
  vTaskSuspend(task_1);
  vTaskSuspend(task_2);
  vTaskSuspend(task_3);
  vTaskSuspend(task_4);

  
}


void loop() {
  task_scheduler();
}

void task_scheduler(){
  vTaskResume(task_1);
  vTaskResume(task_4);
  vTaskDelay(3000/portTICK_PERIOD_MS);
  vTaskSuspend(task_1);

  vTaskResume(task_2);
  vTaskDelay(3000/portTICK_PERIOD_MS);
  vTaskSuspend(task_2);
  
  vTaskResume(task_3);
  vTaskDelay(3000/portTICK_PERIOD_MS);
  vTaskSuspend(task_3);

  count += 1;
}