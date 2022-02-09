#include <stdio.h>
#include <stdbool.h>


/*states of the spending machine */
enum{
    wait_money,
    verificate,
    give_rest,
};
int process_state = wait_money;

/*types of errors */
enum{
    no_error,
    money_insufficient,
    no_coins,
    no_product,
};
int error_state = no_error;


typedef struct 
{
    int coins_1;
    int coins_5;
    int coins_10;
    int coins_50;
    int money;
}coins;

typedef struct
{
    int product_A_num; 
    int product_B_num;
    int product_C_num;

    int product_A_price;
    int product_B_price;
    int product_C_price;

    int basket_price;
}products;

/* the numbers of coins available in the machine */
coins coin_in_the_machine = {
    .coins_1 = 50,
    .coins_5 = 50,
    .coins_10 = 50,
    .coins_50 = 50
};

/* the numbers of coins that the user introduce */
coins coins_user = {
    .coins_1 = 0,
    .coins_5 = 1,
    .coins_10 = 0,
    .coins_50 = 1
};

/* the products available in the machine */
products products_in_the_machine = {
    .product_A_num = 3,
    .product_A_price = 14,

    .product_B_num = 2,
    .product_B_price = 15,

    .product_C_num = 3,
    .product_C_price = 36,
};

/* products to purchase /basket/ */
products basket_user = {
    .product_A_num = 0,
    .product_B_num = 1,
    .product_C_num = 1,
};

/* this function will activate when the user select a product,
 and verificate if the money is enough */
bool money_enough(){
    int money_of_user = wallet_calc(&coins_user); /* hacer que retorne el valor*/
    int basket_price = basket_calc(&basket_user, &products_in_the_machine);
    printf("money_of_user: %d, basket_price: %d\n\r",money_of_user, basket_price);
    if (money_of_user >= basket_price){
        return 1;
    }
    else{
        return 0;
    }
}

/* this function will disconunt money that take for the basket */

void discount_coins_user(coins *coins_user_ptr, coins *coins_machine_ptr){
    int money_of_user = wallet_calc(&coins_user);
    int basket_price = basket_calc(&basket_user, &products_in_the_machine);
    int money_to_discount = basket_price;
    int money_user_post_buy = money_of_user - money_to_discount;
    printf("money_discount: %d, money_post: %d\n\r", money_to_discount, money_user_post_buy);

    int coins_50_user = coins_user_ptr->coins_50;
    int coins_10_user = coins_user_ptr->coins_10;
    int coins_5_user = coins_user_ptr->coins_5;
    int coins_1_user = coins_user_ptr->coins_1;

    int coins_50_machine = coins_machine_ptr->coins_50;
    int coins_10_machine = coins_machine_ptr->coins_10;
    int coins_5_machine = coins_machine_ptr->coins_5;
    int coins_1_machine = coins_machine_ptr->coins_1;

    while (money_to_discount > 0)
    {
        while (money_to_discount - 50 >= 0)
        {
            money_to_discount = money_to_discount - 50;
            coins_50_user = coins_50_user - 1;
            coins_50_machine = coins_50_machine + 1;
        }
        while (money_to_discount - 10 >= 0)
        {
            money_to_discount = money_to_discount - 10;
            coins_10_user = coins_10_user - 1;
            coins_10_machine = coins_10_machine + 1;        
        }
        while (money_to_discount - 5 >= 0)
        {
            money_to_discount = money_to_discount - 5;
            coins_5_user = coins_5_user - 1;
            coins_5_machine = coins_5_machine + 1;
        }
        while (money_to_discount - 1 >= 0)
        {
            money_to_discount = money_to_discount - 1;
            coins_1_user = coins_1_user - 1;
            coins_1_machine = coins_1_machine + 1;
        }
    }
        
    coins_user_ptr->coins_50 = coins_50_user;
    coins_user_ptr->coins_10 = coins_50_user;
    coins_user_ptr->coins_5 = coins_50_user;
    coins_user_ptr->coins_1 = coins_50_user;

    coins_machine_ptr->coins_50 = coins_50_machine;
    coins_machine_ptr->coins_10 = coins_10_machine;
    coins_machine_ptr->coins_5 = coins_5_machine;
    coins_machine_ptr->coins_1 = coins_1_machine;
        
}

/* this function will verificate that coins in the
machine is enough */
bool coins_enough(){
    return 1;
}

/* this function will verificate that the products selected
 is available */
bool products_available(){
    return 1;
}

/* this function calculate the money necessary to buy the basket of the user */
int basket_calc(products *basket_products_ptr, products *price_ptr){
    basket_products_ptr->basket_price = basket_products_ptr->product_A_num * price_ptr->product_A_price
                            + basket_products_ptr->product_B_num * price_ptr->product_B_price
                            + basket_products_ptr->product_C_num * price_ptr->product_C_price;
    return basket_products_ptr->basket_price;
}


/* this function calculate the money in the machine or the user
depending of the argument of the function */
int wallet_calc(coins *s){
    int total_money = s->coins_1 
                    + s->coins_5*5 
                    + s->coins_10*10
                    + s->coins_50*50;
    return total_money;
}

void print_wallets(){
    int money_machine = wallet_calc(&coin_in_the_machine);
    int money_user = wallet_calc(&coins_user);

    printf("money_user: %d, money_machine: %d\n\r", money_user, money_machine);
}

void handle_machine(){
    switch (process_state)
    {
    case wait_money:
        if (money_enough()){
            process_state = verificate;
        }
        else{
            error_state = money_insufficient;
            printf("the money is insufficient\n\r");
            printf("please insert more coins\n\r");
            return;
        }
        break;

    case verificate:
            /* coins enough to give rest */
            if(coins_enough() && products_available()){
                /* descontar dinero de la wallet del usuario y calcular el vuelto */
                /* crear funcion */
                discount_coins_user(&coins_user, &coin_in_the_machine);
                printf("the purchase is done succesfully\n\r");
                process_state = give_rest;
            }
            else{
                if (!coins_enough()){
                    error_state = no_coins;
                    printf("the machine no have sufficient money to give the rest\n");
                }
                if (!products_available()){
                    error_state = no_product;
                    printf("the machine no have the product\n");
                }
            }
        break;
    
    case give_rest:
        printf("new record");
        return;
        break;

    default:
        break;
    }
}



void main(){
    print_wallets();
    handle_machine();
    handle_machine();
    print_wallets();

    
    return 0;
}
