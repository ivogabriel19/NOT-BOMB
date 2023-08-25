/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos de dos digitos
 *                 ESP32 DevKit
*/

//pines escritura segmentos
#define dA 16
#define dB 17
#define dC 18
#define dD 19

//pines escritura anodos segmentos
#define anA 21
#define anB 22
#define anC 23

#define seg 2 //Built in LED

#define wireOK 35
#define wire1 32
#define wire2 33
#define wire3 25
#define wire4 26

#define salto 200

bool flag_stop = false;

int ctrlSegundero = 1000; //Variable control actualizacion del segundero

int contador[] = {0, 0}; // numeros del reloj -> {unidadSegundos , decenaSegundos}

const int pinesDig[] = {dD, dC, dB, dA};  //pines escritura segmentos
const int pinesAn[] = {anC, anB, anA};    //pines escritura anodos segmentos
const int bcdDig[][4] = { //{D, C, B, A}
    {0,0,0,0}, //0
    {0,0,0,1}, //1
    {0,0,1,0}, //2
    {0,0,1,1}, //3
    {0,1,0,0}, //4
    {0,1,0,1}, //5
    {0,1,1,0}, //6
    {0,1,1,1}, //7
    {1,0,0,0}, //8
    {1,0,0,1}  //9
};
const int bcdAn[][3] = {   //{C, B, A}
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 0}
};

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; i++){
    pinMode(pinesDig[i], OUTPUT);
  }
  for(int i = 0; i < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; i++){
    pinMode(pinesAn[i], OUTPUT);
  }
  pinMode(seg ,OUTPUT);
  pinMode(wireOK, INPUT_PULLUP);
  pinMode(wire1, INPUT_PULLUP);
  pinMode(wire2, INPUT_PULLUP);
  pinMode(wire3, INPUT_PULLUP);
  pinMode(wire4, INPUT_PULLUP);
}

void loop() {
  unsigned static long previousMillis = 0;
  unsigned long currentMillis = millis();

  if(!flag_stop){
    checkTime():  //Segundero
    checkWires(); //Deteccion de cables
  }
  refresh();    //Refresco de displays
}

void checkTime(){
  if (currentMillis - previousMillis >= ctrlSegundero) {
      previousMillis = currentMillis;
      segundero();
    }
}

void segundero(){
    // blink segundero
  static bool LEDstate = 0;
  digitalWrite(seg, LEDstate);
  LEDstate = !LEDstate;

  //incremento reloj
  contador[0]++;
  if(contador[0]>9) {
    contador[0]=0;
    contador[1]++;
    if(contador[1]>9){
      contador[1]=0;
    }
  }
  //Serial.println("blink_segundero() " + String(num));
}

void refresh(){//Escribir displays
  //Escribir cada digito del contador
  for(int dig=0 ; dig < (sizeof(contador)/sizeof(contador[0])) ; dig++){
    //Alimento el anodo correspondiente (declarados en pinesAn[])
    for (int anodo=0 ; anodo < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; anodo++){ //3 vueltas
      digitalWrite(pinesAn[anodo], bcdAn[dig][anodo]);
    }
 
    //escribo los segmentos correspondientes (declarados en pinesDig[])
    for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){ //4vueltas
      digitalWrite(pinesDig[pin], bcdDig[contador[dig]][pin]);
    }
    
    //Apago los segmentos
    delay(1);
    for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
      digitalWrite(pinesDig[pin], HIGH);
    }
  }  
}

void checkWires(){
  Serial.println("Segundero: " + String(ctrlSegundero));

  static bool flag_wire1 = false;
  static bool flag_wire2 = false;
  static bool flag_wire3 = false;
  static bool flag_wire4 = false;

  if(!digitalRead(wireOK)){
    flag_stop = true;
    Serial.println("Stop!");
    }

  if(!flag_wire1){
    if(!digitalRead(wire1)){
        ctrlSegundero-=salto;
        if(ctrlSegundero <= 0) ctrlSegundero = 50;
        flag_wire1 = true;
      }
  }

  if(!flag_wire2){
    if(!digitalRead(wire2)){
        ctrlSegundero-=salto;
        if(ctrlSegundero <= 0) ctrlSegundero = 50;
        flag_wire2 = true;
      }
  }

  if(!flag_wire3){
    if(!digitalRead(wire3)){
        ctrlSegundero-=salto;
        if(ctrlSegundero <= 0) ctrlSegundero = 50;
        flag_wire3 = true;
      }
  }

  if(!flag_wire4){
    if(!digitalRead(wire4)){
        ctrlSegundero-=salto;
        if(ctrlSegundero <= 0) ctrlSegundero = 50;
        flag_wire4 = true;
      }
  }
}
