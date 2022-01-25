char command;
int saldo = 0;
const int precio_A = 100;
const int precio_B = 140;


void setup() {
  Serial.begin(9600);
  Serial.println("Bienvenido a la máquina expendedora");
  delay(500);
  Serial.println("Utilice los siguientes comandos para interactuar con ella");
  delay(1000);
  Serial.println("add_1: añade $1 al saldo");
  delay(1000);
  Serial.println("add_1: añade $5 al saldo");
  delay(1000);
  Serial.println("add_1: añade $10 al saldo");
  delay(1000);
  Serial.print("Pa: compra el producto A, valor = "); Serial.println(precio_A);
  delay(1000);
  Serial.print("Pa: compra el producto B, valor = "); Serial.println(precio_B);
  Serial.println("..............................................................");
}

void loop() {
    command = Serial.read();
    if(command == 'A'){
      saldo = saldo + 1;
      Serial.print("Saldo = $");
      Serial.println(saldo);
    }
    
    if(command == 'B'){
      saldo = saldo + 5;
      Serial.print("Saldo = $");
      Serial.println(saldo);
    }
    
    if(command == 'C'){
      saldo = saldo + 10;
      Serial.print("Saldo = $");
      Serial.println(saldo);
    }

    ////////////////////////////////////////////////////   Productos ////////////////////////////////////////////////////

    if(command == 'D'){
      if(saldo >= precio_A){
        saldo = saldo - precio_A;
        Serial.println("Producto A comprado exitosamente.");
        Serial.print("Saldo = $");
        Serial.println(saldo);
      }
      else{
        Serial.println("Saldo insuficiente...");
      }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if(command == 'E'){
      if(saldo >= precio_B){
        saldo = saldo - precio_B;
        Serial.println("Producto B comprado exitosamente.");
        Serial.print("Saldo = $");
        Serial.println(saldo);
      }
      else{
        Serial.println("Saldo insuficiente...");
      }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
}
