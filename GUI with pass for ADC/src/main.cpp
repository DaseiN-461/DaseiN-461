#include <Arduino.h>


#include "gui.h"

int count = 0;
/*
#if CONFIG_FREERTOS_UNICORE
  static const  BaseType_t app_cpu = 0;
#else 
  static const BaseType_t app_cpu = 1;
#endif


static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;


void ask_pass(void *parameter);
void clock(void *parameter);

*/


void setup() {
  tft_init();
  btns_init();
  layout_pass();


/*
  xTaskCreatePinnedToCore(ask_pass,"yellow", 1024, NULL, 2, &task_1, app_cpu);
  
  xTaskCreatePinnedToCore(clock,"yellow", 1024, NULL, 1, &task_2, app_cpu);
  
  vTaskSuspend(task_1);
  vTaskSuspend(task_2);

  vTaskResume(task_2);
*/
}

void loop() {
  handle_machine();
  
}
/*
void ask_pass(void *parameter){
    
}


void clock(void *parameter){
  for(int j=0; j<60; j++){
    for(int i=0; i<60; i++){
        print_clock(j,i);  
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }
}
*/