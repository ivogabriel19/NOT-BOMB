/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos
 *                 NodeMCU 1.0 v2 con display 7 segmentos CA
*/

#define a 2//D4
#define b 0//D3
#define c 4//D2
#define d 5//D1
#define seg 16 //D0

#define INTERVAL 1000

const int pines[] = {d, c, b, a};
//const bool common = true; // True = CA | False = CC
const int bcd[][4] = {
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

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < (sizeof(pines)/sizeof(pines[0])) ; i++){
    pinMode(pines[i], OUTPUT);
    }
  pinMode(seg ,OUTPUT);
}

void loop() {
  unsigned static long previousMillis = 0;    // will store last time DHT was updated
  unsigned long currentMillis = millis();

  //Segundero
  if (currentMillis - previousMillis >= INTERVAL) {
    // save the last time you updated the flag values
    previousMillis = currentMillis;
    segundero();
  }
}

void segundero(){
  static int num = 0;

  // blink segundero
  static bool LEDstate = 0;
  digitalWrite(seg, LEDstate);
  LEDstate = !LEDstate;

  Serial.print("Numero " + String(num) + ": {");
  for (int pin=0 ; pin < (sizeof(pines)/sizeof(pines[0])) ; pin++){
    digitalWrite(pines[pin], bcd[num][pin]);
    Serial.print(String(bcd[num][pin]) + ", ");
    }
  Serial.println("}");

  num++;
  if(num>9) num=0;
  //Serial.println("blink_segundero() " + String(num));
  }
