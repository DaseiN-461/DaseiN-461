#include <Arduino.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#define btn_left_pin 0
#define btn_right_pin 35
#define ADC 2

const int8_t xpos_digit1 = 15;
const int8_t ypos_digit1 = 35;

const int8_t xpos_digit2 = 45;
const int8_t ypos_digit2 = 35;

const int8_t xpos_digit3 = 75;
const int8_t ypos_digit3 = 35;

const int8_t xpos_digit4 = 105;
const int8_t ypos_digit4 = 35;

int pass[4] = {1,0,1,0};
bool flg_pass = false;


TFT_eSPI tft = TFT_eSPI();

bool current_state_left_btn = false;
bool last_state_left_btn = true;

bool current_state_right_btn = false;
bool last_state_right_btn = true;

int num = 0;

enum{
    first,
    second,
    third,
    last,
};
int digit = first;


typedef struct{
    int first_num;
    int second_num;
    int third_num;
    int last_num;
}code;

code code_in = {.first_num=0, .second_num=0, .third_num=0, .last_num=0};

////////////////////////////////////Declare functions //////////////////////////////////////////
void tft_init();
void btns_init();

void btn_left_push();
void btn_left_do();
void btn_right_push();
void btn_right_do(code *ptr);

void verificate_code(code *ptr);
void layout_pass();
//////////////////////////////////////////////////////////////////////////////////////////////////////


void tft_init(){
  tft.begin();
  tft.setRotation(4);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
}

void print_clock(int j,int i){
    tft.fillRect(0,0,60,25,TFT_DARKGREY);
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(0,0);
    tft.print(j);
    tft.print(":");
    tft.print(i);
}


void btns_init(){
    pinMode(btn_left_pin, INPUT);
    pinMode(btn_right_pin, INPUT);
    pinMode(ADC,INPUT);
}




void btn_left_push(){
  current_state_left_btn = digitalRead(btn_left_pin);
  if(current_state_left_btn != last_state_left_btn){
      if(current_state_left_btn == true){
          btn_left_do();
      }
      delay(50);
  }
  last_state_left_btn = current_state_left_btn;
}

void btn_left_do(){
    if(num == 9){
        num = 0;
    }else{
        num++;
    }
    switch (digit){
    case first:
        tft.setCursor(xpos_digit1,ypos_digit1);
        tft.fillRect(10,30,20,30,TFT_WHITE);
        tft.print(num);
        break;

    case second:
        tft.setCursor(xpos_digit2,ypos_digit2);
        tft.fillRect(40,30,20,30,TFT_WHITE);
        tft.print(num);
        break;

    case third:
        tft.setCursor(xpos_digit3,ypos_digit3);
        tft.fillRect(70,30,20,30,TFT_WHITE);
        tft.print(num);
        break;
    
    case last:
        tft.setCursor(xpos_digit4,ypos_digit4);
        tft.fillRect(100,30,20,30,TFT_WHITE);
        tft.print(num);
        break;
    }
}



void btn_right_push(){
  current_state_right_btn = digitalRead(btn_right_pin);
  if(current_state_right_btn != last_state_right_btn){
      if(current_state_right_btn == true){
          btn_right_do(&code_in);
      }
      delay(50);
  }
  last_state_right_btn = current_state_right_btn;
}

void btn_right_do(code *ptr){
    switch(digit){
        case first:
            tft.setCursor(xpos_digit1,ypos_digit1);
            tft.print(num);
            ptr->first_num = num;
            num = 0;
            digit = second;

            break;

        case second:
            tft.setCursor(xpos_digit2,ypos_digit2);
            tft.print(num);
            ptr->second_num = num;
            num = 0;
            digit = third;

            break;

        case third:
            tft.setCursor(xpos_digit3,ypos_digit3);
            tft.print(num);
            ptr->third_num = num;
            num = 0;
            digit = last;

            break;

        case last:
            tft.setCursor(xpos_digit4,ypos_digit4);
            tft.print(num);
            ptr->last_num = num;
            num = 0;
            


            verificate_code(&code_in);

            break;
    }
}

void verificate_code(code *ptr){
    int first = ptr->first_num;
    int second = ptr->second_num;
    int third = ptr->third_num;
    int last = ptr->last_num;

    if(first == pass[0]){
        if(second == pass[1]){
            if(third == pass[2]){
                if(last == pass[3]){
                    flg_pass = true;
                    tft.fillScreen(TFT_BLACK);
                    tft.setTextColor(TFT_WHITE);
                    tft.setCursor(0,0);
                    tft.setTextSize(2);
                    tft.print("ADC read: ");  
                    tft.setCursor(0,100);
                    tft.setTextSize(1);
                    tft.print("press any button to exit"); 
                    tft.setTextSize(2); 
                }
            }
        }
    }else{
        tft.fillScreen(TFT_BLACK);
        layout_pass();
        digit = first;
    }


}

void handle_machine(){
    if(flg_pass == false){
        btn_left_push();
        btn_right_push();
    }else{
        tft.setCursor(35,35);
        tft.fillRect(30,30,60,30,TFT_BLACK);
        tft.print(analogRead(ADC));
        delay(100);
        current_state_left_btn = digitalRead(btn_left_pin);
        if(current_state_left_btn != last_state_left_btn){
            if(current_state_left_btn == true){
                tft.fillScreen(TFT_BLACK);
                layout_pass();
                tft.setTextColor(TFT_BLACK);
                digit = first;
                flg_pass = false;             
                return;
            }
            delay(50);
        }
        last_state_left_btn = current_state_left_btn;
    }
}

void layout_pass(){
  tft.fillRect(10,30,20,30,TFT_WHITE);
  tft.fillRect(40,30,20,30,TFT_WHITE);
  tft.fillRect(70,30,20,30,TFT_WHITE);
  tft.fillRect(100,30,20,30,TFT_WHITE);
}