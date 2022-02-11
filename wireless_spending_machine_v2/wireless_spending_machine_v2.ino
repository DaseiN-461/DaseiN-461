#include <WiFi.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  
WiFiServer server(80);



enum{
  wait,
  verificate,
  availability,
  give_rest,
};
int state = wait;

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


const char* ssid     = "wifi_CARA_2.4";
const char* password = "18112000";
 
int contconexion = 0;
int val = 0;
String state_msg = "";
bool flag_no_coins = 0;


String header; //cadena de caracteres correspondiente a la instruccion


void setup_screen(){
  tft.init();
  tft.setRotation(4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
}


void update_wallet(coins *ptr){
  int coins_1 = ptr->coins_1;
  int coins_5 = ptr->coins_5;
  int coins_10 = ptr->coins_10;
  int money = wallet_calc(&coins_user);
  
  tft.println("----------");
  tft.println("- Wallet -");
  tft.println("----------");
  tft.print("coins of 1:  ");
  tft.println(coins_1); 
  tft.print("coins of 5:  ");
  tft.println(coins_5);
  tft.print("coins of 10: ");
  tft.println(coins_10);
  tft.println("----------");
  tft.print("money:      $");
  tft.println(money);
  tft.println("----------");
}

void update_basket(products *ptr_user, products *ptr_machine){
  int num_A = ptr_user->A_num;
  int price_A = ptr_machine->A_price;
  int total = num_A * price_A;
  
  tft.println("----------");
  tft.println("- Basket -");
  tft.println("----------");
  tft.print("products A:  ");
  tft.println(num_A); 
  tft.println("----------");
  tft.print("total:      $");
  tft.println(total);
  tft.println("----------");
}


void update_screen(){
  tft.fillScreen(TFT_BLACK);  
  tft.setCursor(0,0);

  tft.println("Wireless");
  tft.println("Spending machine");

  tft.println();
  tft.println();

  
  update_wallet(&coins_user);
  
  tft.println();
  tft.println();
  
  update_basket(&basket_user, &products_in_the_machine);
  
  tft.println();
  tft.println();
  tft.println();

  tft.println(state_msg);

  
}




//Funciones relacionadas con la máquina
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
        tft.println("no coins of 10");
        flag_no_coins = 1;
      }     
    }
    if(resto >= 5){
      if(machine_5 > 1){
        user_5 = user_5 + 1;
        machine_5 = machine_5 - 1;
        resto = resto - 5;
      }else{
        tft.println("no coins of 5");
        flag_no_coins = 1;
      }     
    }
    if(resto >= 1){
      if(machine_1 > 1){
        user_1 = user_1 + 1;
        machine_1 = machine_1 - 1;
        resto = resto - 1;
      }else{
        tft.println("no coins of 1");
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

bool money_enough(){
  int money = wallet_calc(&coins_user);
  int price = basket_price(&basket_user, &products_in_the_machine);
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



void handle_request(){
  WiFiClient client = server.available();
  if(client){
    //new client
    String  current_line = "";
    while(client.connected()){
      if(client.available()){ //si hay un dato en el buffer
        char c = client.read(); // lo guarda en c
        header += c; // se concatena al final de la cadena header
        if(c == '\n'){ // si c es un salto de linea
          if(current_line.length() == 0){
//add coins
            if(header.indexOf("GET /A") >= 0){
              add_coins(&coins_user,1);
              update_screen();
            }
            if(header.indexOf("GET /B") >= 0){
              add_coins(&coins_user,5);
              update_screen();  
            }
            if(header.indexOf("GET /C") >= 0){
              add_coins(&coins_user,10);
              update_screen();  
            }
///add products
            if(header.indexOf("GET /D") >= 0){
              add_product(&basket_user);
              update_screen();
            }
//process the buy
            if(header.indexOf("GET /E") >= 0){
              state = verificate;
            }
                        
            // Aquí empieza a terminar la comunicación con el cliente
            client.println();  //envía una linea en blanco
            break;
          }else{                     //si la longitud de current_line != 0
            current_line = "";       //si había una linea, la borra para comenzar una nueva comunicacion
          }
          
        }else if(c != '\r'){ // si c es dinstinto del retorno de carro
          current_line += c; // llena current_line con los caracteres hasta completar el comando (\r\n)
        }
      }
    }
    //cuando sale del while(client.connected())
    header = "";
    client.stop(); //termina la conexión
  }
}

void wifi_begin(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      Serial.println("");
      Serial.println("WiFi conectado"); 
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

void handle_machine(){
  switch(state){
    case wait:
      handle_request();  
      break;
      
    case verificate:
      if(money_enough()){
        state = availability;
      }else{
        state_msg = "insufficient coins";
        update_screen();
        delay(1000);
        state_msg = "";
        update_screen();
        state = wait;        
      }
      break;
      
    case availability:
      if(coins_enough() && products_available()){
        state = give_rest;
      }else{
        //terminar de crear
        state_msg = "problem with machine";
        update_screen();
        delay(1000);
        state_msg = "";
        update_screen();
        state = wait;
      }
      break;
      
    case give_rest:
      trade_coins(&coins_user, &coins_in_the_machine, &basket_user, &products_in_the_machine);
      state_msg = "Sucessfully";
      update_screen();
      delay(1000);
      state_msg = "";
      update_screen();
      state = wait;
      break;
  }
}

void setup() {
  wifi_begin();
  setup_screen();
  update_screen();
}

void loop() {
  delay(100);
  handle_machine();
}
