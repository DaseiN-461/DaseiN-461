enum{
    wait_money,
    buying,
    give_rest,
};
int state = wait_money;


typedef struct //coins struct
{
    int8_t coins_1;
    int8_t coins_5;
    int8_t coins_10;
}coins;

typedef struct // products struct
{
    int8_t A_num; 
    int8_t A_price;
    
}products;

coins coins_in_the_machine = {
    .coins_1 = 50,
    .coins_5 = 50,
    .coins_10 = 50,
};
coins coins_user = {
    .coins_1 = 0,
    .coins_5 = 0,
    .coins_10 = 0,
};


products basket_user = {
    .A_num = 0,
};
products products_in_the_machine = {
    .A_num = 20,
    .A_price = 13,

};

bool flag_buy = 0;
bool flag_basket = 0;
bool flag_no_coins = 0;

int wallet_calc(coins *ptr){
  int money = ptr->coins_1 + ptr->coins_5*5 + ptr->coins_10*10;
  return money;
}

int basket_price(products *ptr_num, products *ptr_price){
  int price = ptr_num->A_num*ptr_price->A_price;
  return price;
}

void show_wallet(coins *ptr){
  int8_t coins_1 = ptr->coins_1;
  int8_t coins_5 = ptr->coins_5;
  int8_t coins_10 = ptr->coins_10;
  Serial.println("------------------------------------");
  Serial.println("the wallet have: ");
  
  Serial.print("coins of 1: ");
  Serial.println(coins_1);
  
  Serial.print("coins of 5: ");
  Serial.println(coins_5);
  
  Serial.print("coins of 10: ");
  Serial.println(coins_10);
  Serial.println("------------------------------------");
}


bool input_coins(coins *ptr_user,coins *ptr_machine){
  while(Serial.available() == 0){
  }
  char command = Serial.read();
  
  switch(command){
    case 'A': // to introduce 1 coin
      ptr_user->coins_1 = ptr_user->coins_1 + 1;
      Serial.println("add 1 coin");
      break;
    case 'B': // to introduce 5 coins
      ptr_user->coins_5 = ptr_user->coins_5 + 1;
      Serial.println("add 5 coins");
      break;
    case 'C': // to introduce 10 coins
      ptr_user->coins_10 = ptr_user->coins_10 + 1;
      Serial.println("add 10 coins");
      break;
    case 'D':
      show_wallet(&coins_user);
      flag_buy = 1;
  }
}

void select_basket(products *ptr){
  while(Serial.available() == 0){
  }
  char command = Serial.read();
  switch(command){
    case 'A':
      ptr->A_num = ptr->A_num + 1;
      Serial.println("add product A");
      break;
    case 'B':
      flag_basket = 1;
      Serial.println("procesing the buy...");
      Serial.println("------------------------------------");
      break;
  }
}


bool money_enough(){
  int money = wallet_calc(&coins_user);
  int price = basket_price(&basket_user, &products_in_the_machine);
  Serial.print("user_money: ");
  Serial.println(money);
  Serial.print("basket_price: ");
  Serial.println(price);
  if(money >= price){
    return 1;
  }else{
    return 0;
  }
}

bool coins_enough(){
  return 1;
}

bool products_available(){
  
  return 1;
}

void show_basket(products *ptr){
  int num_A = ptr->A_num;

  Serial.println("la canasta tiene: ");
  Serial.print("#productos A: ");
  Serial.println(num_A);
  Serial.println("------------------------------------");
}



void trade_coins(coins *ptr_user, coins *ptr_machine, products *ptr_basket, products *ptr_products){
  uint8_t user_1 = ptr_user->coins_1;
  uint8_t user_5 = ptr_user->coins_5;
  uint8_t user_10 = ptr_user->coins_10;

  uint8_t machine_1 = ptr_machine->coins_1;
  uint8_t machine_5 = ptr_machine->coins_5;
  uint8_t machine_10 = ptr_machine->coins_10;

  uint8_t A_num_user = ptr_basket->A_num;
  uint8_t A_num_machine = ptr_products->A_num;

  uint8_t money = wallet_calc(&coins_user);
  uint8_t price = basket_price(&basket_user, &products_in_the_machine);

  uint8_t resto = money - price;

  //primero que todo se traspasan todas las monedas
  machine_1 = machine_1 + user_1;
  machine_5 = machine_5 + user_5;
  machine_10 = machine_10 + user_10;

  user_1 = 0;
  user_5 = 0;
  user_10 = 0;
  
  while(resto > 0){
    if(resto >= 10){
      if(machine_10 > 1){
        user_10 = user_10 + 1;
        machine_10 = machine_10 - 1;
        resto = resto - 10;
      }else{
        Serial.println("no coins of 10");
        flag_no_coins = 1;
      }     
    }
    if(resto >= 5){
      if(machine_5 > 1){
        user_5 = user_5 + 1;
        machine_5 = machine_5 - 1;
        resto = resto - 5;
      }else{
        Serial.println("no coins of 5");
        flag_no_coins = 1;
      }     
    }
    if(resto >= 1){
      if(machine_1 > 1){
        user_1 = user_1 + 1;
        machine_1 = machine_1 - 1;
        resto = resto - 1;
      }else{
        Serial.println("no coins of 1");
        flag_no_coins = 1;
      }     
    }
  }
  ptr_products->A_num = A_num_machine - A_num_user;
  ptr_basket->A_num = 0;
  
  ptr_user->coins_1 = user_1;
  ptr_user->coins_5 = user_5;
  ptr_user->coins_10 = user_10;

  ptr_machine->coins_1 = machine_1;
  ptr_machine->coins_5 = machine_5;
  ptr_machine->coins_10 = machine_10;
  
}





void give_coins(){

}

void msg_start(){

  
  Serial.println("introduce the coins...");
  Serial.println("A: coin 1");
  Serial.println("B: coin 5");
  Serial.println("C: coin 10");
  Serial.println("D: to next step (select products)");
  Serial.println("");

  Serial.println("------------------------------------");
  
}

void msg_basket(){
  Serial.println("preparing the basket...");
  Serial.println("A: add product A");
  Serial.println("B: to next step (end the purchase)");
  Serial.println("------------------------------------");
}

void handle_machine(){
  switch(state){
    case wait_money:
      input_coins(&coins_user, &coins_in_the_machine);
      if(flag_buy){
        msg_basket();
        state = buying;
      }
      break;
      
    case buying:
      flag_buy = 0;
      select_basket(&basket_user);    
      if(flag_basket){
        if(money_enough()){         
          if(products_available()){
            trade_coins(&coins_user, &coins_in_the_machine, &basket_user, &products_in_the_machine);
            if(flag_no_coins){
              state = wait_money;
            }
            state = give_rest;
          }else{
              Serial.println("no hay productos suficientes en la maquina");
              state = wait_money;
            }
        }
         else{
          Serial.println("------------------------------------");
          Serial.println("dinero insuficiente");
          Serial.println("sus productos han sido guardado exitosamente");
          Serial.println("ingrese más monedas para poder completar la compra");
          Serial.println("------------------------------------");
          msg_start();
          state = wait_money;
        }   } 
      
      break;
      
    case give_rest:
      flag_basket = 0;
      Serial.println("------------------------------------");
      show_wallet(&coins_user);
      give_coins();
      Serial.println("compra realizada con exito");
      state = wait_money;
      break;
  }
}



void setup() {
  
  Serial.begin(9600);
  Serial.println("welcome to spending machine: ");
  Serial.println("");
  msg_start();
  

}
  
  
void loop() {
  handle_machine();
}
