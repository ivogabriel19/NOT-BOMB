//pines deteccion cables
#define wireOK 36
#define wire1 39
#define wire2 34
#define wire3 35
#define wire4 32
#define wire5 33
#define wire6 25
#define wire7 26
#define wire8 27

#define INTERVALO 800

float salto = 200;
float err = 1.2 ;

unsigned long previousMillis = 0;
int ctrlSegundero = 1000; //Variable control actualizacion del segundero

const int pinesWr[] = {wireOK, wire1, wire2, wire3, wire4, wire5, wire6, wire7, wire8}; //pines deteccion cables

void setup() {
  Serial.begin(115200);

  for(int pin = 0; pin < (sizeof(pinesWr)/sizeof(pinesWr[0])) ; pin++){
    pinMode(pinesWr[pin], INPUT);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= INTERVALO) {
      previousMillis = currentMillis;
      checkWires();
    }
}

void checkWires(){

  static bool flag_wire[] = {0, false, false, false, false,
                             false, false, false, false};

  if(!digitalRead(wireOK)){
    //tutifruti();
  }

  for(int wire = 1; wire < (sizeof(pinesWr)/sizeof(pinesWr[0])) ; wire++){
    if(!flag_wire[wire]){
      if(!digitalRead(pinesWr[wire])){
          ctrlSegundero-=salto;
          salto*=err;
          if(ctrlSegundero <= 0) ctrlSegundero = 50;
          flag_wire[wire] = true;

          Serial.println(" Wire "+ String(wire) +" detected!");
      }
    }
  }
}

void checkWires2(){
  Serial.println();
  Serial.println();
  for(int pin = 0; pin < (sizeof(pinesWr)/sizeof(pinesWr[0])) ; pin++){
    Serial.println("Wire " + String(pin) + ": " + String(digitalRead(pinesWr[pin])) );
  }
  Serial.println();
  Serial.println();
}
