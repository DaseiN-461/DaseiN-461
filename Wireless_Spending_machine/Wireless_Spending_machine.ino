#include <WiFi.h>

#include "pagina.h"
#include "basket.h"
#include "spending_machine.h"
#include "screen.h"




//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

// credenciales hotspot mobile
/*
const char* ssid     = "spending_net";
const char* password = "spending_pass";

*/
const char* ssid     = "wifi_CARA_2.4";
const char* password = "18112000"; 


//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;
bool flag_no_coins = 0;

String header; // Variable para guardar el HTTP request byte a byte se escribe hasta recibir \n

void show_wallet(coins *ptr){
  int8_t coins_1 = ptr->coins_1;
  int8_t coins_5 = ptr->coins_5;
  int8_t coins_10 = ptr->coins_10;
  tft.println("------------------------------------");
  tft.println("the wallet have: ");
  
  tft.print("coins of 1: ");
  tft.println(coins_1);
  
  tft.print("coins of 5: ");
  tft.println(coins_5);
  
  tft.print("coins of 10: ");
  tft.println(coins_10);
  tft.println("------------------------------------");
}

bool money_enough(){
  int money = wallet_calc(&coins_user);
  int price = basket_price(&basket_user, &products_in_the_machine);
  tft.print("user_money: ");
  tft.println(money);
  tft.print("basket_price: ");
  tft.println(price);
  if(money >= price){
    return 1;
  }else{
    return 0;
  }
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



void handle_request(){
      WiFiClient client = server.available();   // Escucha a los clientes entrantes
      if (client) {                             // Si se conecta un nuevo cliente
        Serial.println("New Client.");          // 
        String currentLine = "";                //
        while (client.connected()) {            // loop mientras el cliente está conectado
          if (client.available()) {             // si hay bytes para leer desde el cliente
            char c = client.read();             // lee un byte
            Serial.write(c);                    // imprime ese byte en el monitor serial
            header += c;
            if (c == '\n') {                    // si el byte es un caracter de salto de linea
              // si la nueva linea está en blanco significa que es el fin del 
              // HTTP request del cliente, entonces respondemos:
              if (currentLine.length() == 0) {
              /*client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();*/
                
                // Lista de comandos
                if (header.indexOf("GET /coin_one") >= 0) {
                  tft.println("add 1 coin");
                  add_coins(&coins_user,1);
                }
                if (header.indexOf("GET /coin_five") >= 0) {
                  tft.println("add 5 coins");
                  add_coins(&coins_user,5);
                }
                if (header.indexOf("GET /coin_ten") >= 0) {
                  tft.println("add 10 coins");
                  add_coins(&coins_user,10);
                }
                
                if (header.indexOf("GET /basket") >= 0) {
                  client.println(basket);
                  if(header.indexOf("GET /basket/p_a") >= 0){
                    add_product(&basket_user);
                    tft.println("add 1 product A");
                  }
                  if(header.indexOf("GET /basket/buy") >= 0){
                    if(money_enough()){
                      trade_coins(&coins_user, &coins_in_the_machine, &basket_user, &products_in_the_machine);
                      tft.println("the trade is done sucessfully");
                      delay(1000);
                      tft.fillScreen(TFT_BLACK);
                      tft.setCursor(0,0);
                      delay(1000);
                      show_wallet(&coins_user);
                      tft.println("ready for the next buy");
                    }else{
                      tft.println("insufficient coins");
                    }
                  }
                }
                
                // Muestra la página web
                client.println(pagina);
                
                // la respuesta HTTP temina con una linea en blanco
                client.println();
                break;
              } else { // si tenemos una nueva linea limpiamos currentLine
                currentLine = "";
              }
            } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
              currentLine += c;      // lo agrega al final de currentLine
            }
          }
        }
        // Limpiamos la variable header
        header = "";
        // Cerramos la conexión
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
      }
  }




//---------------------------SETUP--------------------------------
void setup() {
  setup_screen();
  Serial.begin(115200);
  Serial.println("");
  

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado"); 
      Serial.println(WiFi.localIP());
      tft.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop(){
  handle_request();
}
