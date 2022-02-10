enum{
    wait_money,
    buying,
    give_rest,
};
int state = wait_money;


typedef struct //coins struct
{
    uint8_t coins_1;
    uint8_t coins_5;
    uint8_t coins_10;
}coins;

typedef struct // products struct
{
    uint8_t A_num; 
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

void add_coins(coins *ptr, int num_coins){
  switch(num_coins){
    case 1:
      ptr->coins_1 = ptr->coins_1 + 1;
      break;
    case 5:
      ptr->coins_5 = ptr->coins_5 + 1;
      break;
    case 10:
      ptr->coins_10 = ptr->coins_10 + 1;
      break;
  }
}
void add_product(products *ptr){
  ptr->A_num = ptr->A_num + 1;
}

int wallet_calc(coins *ptr){
  int money = ptr->coins_1 + ptr->coins_5*5 + ptr->coins_10*10;
  return money;
}

int basket_price(products *ptr_num, products *ptr_price){
  int price = ptr_num->A_num*ptr_price->A_price;
  return price;
}
