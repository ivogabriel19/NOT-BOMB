/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos de dos digitos
 *                 ESP32 DevKit
*/

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


//pines escritura segmentos
#define dA  5
#define dB 21
#define dC 19
#define dD 18

//pines escritura anodos segmentos
#define anA 16
#define anB  4
#define anC  2

#define seg  2 //Built in LED
#define dp   17
#define p1   22
#define p2   23

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

float salto = 200;
float err = 1.2 ;

bool flag_stop = false;
unsigned long previousMillis = 0;

int ctrlSegundero = 1000; //Variable control actualizacion del segundero

int contador[] = {0, 5, 3, 0, 0}; // numeros del reloj -> {unidadSegundos, decenaSegundos, unidadMinutos, decenaMinutos, unidadHoras}

const int pinesDig[] = {dD, dC, dB, dA};  //pines escritura segmentos
const int pinesAn[] = {anC, anB, anA};    //pines escritura anodos segmentos
const int pinesWr[] = {wireOK, wire1, wire2, wire3, wire4, wire5, wire6, wire7, wire8}; //pines deteccion cables //En caso de agregar cables, extender manualmente el array de flags en la funcion checkWires()
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
  {1, 0, 1}, // digito 1 - Y5
  {1, 0, 0}, // digito 2 - Y4
  {0, 1, 1}, // digito 3 - Y3
  {0, 1, 0}, // digito 4 - Y2
  {0, 0, 1}  // digito 5 - Y1
};

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Mod Control");
  
  for(int pin = 0; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
    pinMode(pinesDig[pin], OUTPUT);
  }
  for(int pin = 0; pin < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; pin++){
    pinMode(pinesAn[pin], OUTPUT);
  }
  for(int pin = 0; pin < (sizeof(pinesWr)/sizeof(pinesWr[0])) ; pin++){
    pinMode(pinesWr[pin], INPUT);
  }

  pinMode(seg ,OUTPUT);
  pinMode(dp ,OUTPUT);
  pinMode(p1 ,OUTPUT);
  pinMode(p2 ,OUTPUT);
}

void loop() {

  if(!flag_stop){
    checkTime();  //Segundero
    checkWires(); //Deteccion de cables
  }

  refresh();    //Refresco de displays
}

void checkTime(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= ctrlSegundero) {
      previousMillis = currentMillis;
      segundero();
      SerialBT.println (flag_stop);
    }
}

void segundero(){
    // blink segundero
  static bool LEDstate = 0;
  digitalWrite(seg, LEDstate);
  LEDstate = !LEDstate;

  //decremento reloj
  contador[0]--;
  if(contador[0]<0) {
    contador[0]=9;
    contador[1]--;
    if(contador[1]<0){
      contador[1] = 5;
      contador[2]--;
      if(contador[2]<0){
        contador[2]=9;
        contador[3]--;
        if(contador[3]<0){
          contador[3] = 5;
          contador[4]--;
          if (contador[4]<0){
            contador[0]=0;
            contador[1]=0;
            contador[2]=0;
            contador[3]=0;
            tutifruti();    
          }
        }  
      }
    }
  }
  //Serial.println("blink_segundero() " + String(num));
}

void refresh(){//Escribir displays

  if(!flag_stop){ //Va por la actualizacion normal
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
  } else { //Si termino titila en 0
    
    while(true){
      //Escribir cada digito del contador
      // !!! el bcd7segmentos lo escribe la funcion que activa el flag_stop
      for(int dig=0 ; dig < (sizeof(contador)/sizeof(contador[0])) ; dig++){
        //Alimento cada anodo (declarados en pinesAn[])
        for (int anodo=0 ; anodo < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; anodo++){ //3 vueltas
          digitalWrite(pinesAn[anodo], bcdAn[dig][anodo]);
        }
      }
    }
  }
}

void checkWires(){
  Serial.println("Segundero: " + String(ctrlSegundero));

  static bool flag_wire[] = {0, false, false, false, false,
                             false, false, false, false};

  if(!digitalRead(wireOK)){
    tutifruti();
    }

  for(int wire = 1; wire < (sizeof(pinesWr)/sizeof(pinesWr[0])) ; wire++){    // Recorre array de cables
    if(!flag_wire[wire]){                                                     // Si todavia no salto el flag
      if(!digitalRead(pinesWr[wire])){                                        // Entonces hace la lectura
          flag_wire[wire] = true;                                             // Si detecta que no esta el cable activa el flag correspondiente
          ctrlSegundero-=salto;                                               // Acelera el segundero
          salto*=err;                                                         // Aumenta la variable de ajuste del segundero (asi al siguiente error lo acelera mas)
          if(ctrlSegundero <= 0) ctrlSegundero = 50;                          // Si no queda mas para reducir lo fija en 50          
          //Serial.println(" Wire "+ String(wire) +" detected!");
      }
    }
  }  
}

void tutifruti(){ //canta basta para mi basta para todos
  flag_stop = true;
  Serial.println("Stop!");
  //escribo los segmentos en 0 (declarados en pinesDig[])
  for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){ //4vueltas
    digitalWrite(pinesDig[pin], bcdDig[0][pin]);
  }
}
