const char text[11] = "hello world";

void char_sweap(){
  for(int i = 0; i < 11; i++){
    char out[10];
    Serial.print(text[i]);
    Serial.print(" -> ");
    itoa(text[i],out,2);
    Serial.println(out);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(text);
  char_sweap();
}

void loop() {
  // put your main code here, to run repeatedly:

}
