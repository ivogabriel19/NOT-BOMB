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
    {1,0,0,1}, //9
    {1,1,1,1}  // off
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
}

void loop() {
  refresh();    //Refresco de displays
}

void refresh(){//Escribir displays

  int num = 6;
  
    //Escribir cada digito del contador
    for(int dig=0 ; dig < 5 ; dig++){

      Serial.println(num);
  
      //escribo los segmentos correspondientes (declarados en pinesDig[])
      for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){ //4vueltas
        digitalWrite(pinesDig[pin], bcdDig[num][pin]);
      }
      
      //Alimento el anodo correspondiente (declarados en pinesAn[])
      for (int anodo=0 ; anodo < (sizeof(pinesAn)/sizeof(pinesAn[0])) ; anodo++){ //3 vueltas
        digitalWrite(pinesAn[anodo], bcdAn[dig][anodo]);
      }
         
      //Apago los segmentos
      delay(1);
      for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
        digitalWrite(pinesDig[pin], HIGH);
      }
    }
}
