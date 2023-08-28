//Cuenta regresiva para chequear los digitos, tanto los 7 segmentos como las anodos correspondientes

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

bool flag_stop = false;
unsigned long previousMillis = 0;

int ctrlSegundero = 1000; //Variable control actualizacion del segundero

int contador[] = {0, 1, 1, 0, 1}; // numeros del reloj -> {unidadSegundos, decenaSegundos, unidadMinutos, decenaMinutos, unidadHoras}

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
  {1, 0, 1}, // digito 1 - Y5
  {1, 0, 0}, // digito 2 - Y4
  {0, 1, 1}, // digito 3 - Y3
  {0, 1, 0}, // digito 4 - Y2
  {0, 0, 1}  // digito 5 - Y1
};

void setup() {
  Serial.begin(115200);
  
  for(int pin = 0; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
    pinMode(pinesDig[pin], OUTPUT);
  }
  for(int pin = 0; pin < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; pin++){
    pinMode(pinesAn[pin], OUTPUT);
  }

  pinMode(seg, OUTPUT);
  pinMode(dp, OUTPUT);
  digitalWrite(dp, HIGH);
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
}

void loop() {

  if(!flag_stop){
    checkTime();  //Segundero
  }

  refresh();    //Refresco de displays
}

void checkTime(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= ctrlSegundero) {
      previousMillis = currentMillis;
      segundero();
    }
}

void segundero(){
  
  Serial.println("Segundero: " + String(ctrlSegundero));
  
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

      Serial.print(contador[dig]);
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
    Serial.println();
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

void tutifruti(){ //canta basta para mi basta para todos
  flag_stop = true;
  Serial.println("Stop!");
  //escribo los segmentos en 0 (declarados en pinesDig[])
  for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){ //4vueltas
    digitalWrite(pinesDig[pin], bcdDig[0][pin]);
  }
}
