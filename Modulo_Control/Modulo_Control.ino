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

int ctrlSegundero = 1000; //Variable control actualizacion del segundero


int contador[] = {0, 0}; // numeros del reloj -> {unidadSegundos , decenaSegundos}

const int pinesDig[] = {dD, dC, dB, dA};  //pines escritura segmentos
const int pinesAn[] = {anC, anB, anA};    //pines escritura anodos segmentos
const int bcdDig[][4] = {
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
const int bcdAn[][3] = {
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
}

void loop() {
  unsigned static long previousMillis = 0;
  unsigned long currentMillis = millis();

  //Segundero
  if (currentMillis - previousMillis >= ctrlSegundero) {
    previousMillis = currentMillis;
    segundero();
  }

  refresh();
}

void segundero(){

  Serial.println("Segundero: " + String(ctrlSegundero));
  ctrlSegundero-=50;
  if(ctrlSegundero <= 0) ctrlSegundero = 50;

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
  //Escribir cada digito
  for(int dig=0 ; dig < 2 ; dig++){
    //Alimento el anodo correspondiente
    digitalWrite(anA, bcdAn[dig][2]);
    digitalWrite(anB, bcdAn[dig][1]);
    digitalWrite(anC, bcdAn[dig][0]);
 
    //escribo los segmentos correspondientes
    //Serial.print("Cuenta:" + String(dig2) + String(dig1) + " Digito 1 Numero " + String(dig1) + ": {");
    for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
      digitalWrite(pinesDig[pin], bcdDig[contador[dig]][pin]);
      //Serial.print(String(bcdDig[dig1][pin]) + ", ");
      }
    //Serial.println("}");
    
    //Apago los segmentos
    delay(1);
    for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
      digitalWrite(pinesDig[pin], 1);
      }
   }  
}
