/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos de dos digitos
 *                 ESP32 DevKit
*/

#define dA 16
#define dB 17
#define dC 18
#define dD 19

#define anA 21
#define anB 22
#define anC 23

#define seg 2 //Built in LED

#define INTERVAL 1000

const int pinesDig[] = {dD, dC, dB, dA};
const int pinesAn[] = {anA, anB, anC};
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
  pinMode(seg ,OUTPUT);
  pinMode(t1 ,OUTPUT);
  pinMode(t2 ,OUTPUT);

  digitalWrite(t1, LOW);
  digitalWrite(t2, HIGH  );
}

void loop() {
  unsigned static long previousMillis = 0;    // will store last time DHT was updated
  unsigned long currentMillis = millis();
  static int dig1 = 0;
  static int dig2 = 0;

  //Segundero
  if (currentMillis - previousMillis >= INTERVAL) {
    // save the last time you updated the flag values
    previousMillis = currentMillis;
    segundero();
  }

  //Escribir displays
  //Escribo primer digito
  digitalWrite(anA, LOW);
  digitalWrite(anB, LOW);
  digitalWrite(anC, LOW);
  Serial.print("Numero " + String(dig1) + ": {");

  for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
    digitalWrite(pinesDig[pin], bcdDig[dig1][pin]);
    Serial.print(String(bcdDig[dig1][pin]) + ", ");
    }
  
  Serial.println("}");
  delay(100);

  
  //Escribo primer segundo
  digitalWrite(anA, HIGH);
  digitalWrite(anB, LOW);
  digitalWrite(anC, LOW);
  Serial.print("Numero " + String(dig2) + ": {");

  for (int pin=0 ; pin < (sizeof(pinesDig)/sizeof(pinesDig[0])) ; pin++){
    digitalWrite(pinesDig[pin], bcdDig[dig2][pin]);
    Serial.print(String(bcdDig[dig2][pin]) + ", ");
    }
  
  Serial.println("}");
  delay(100);
}

void segundero(){

  // blink segundero
  static bool LEDstate = 0;
  digitalWrite(seg, LEDstate);
  LEDstate = !LEDstate;

  dig1++;
  if(dig1>9) dig1=0;
  //Serial.println("blink_segundero() " + String(num));
  }
