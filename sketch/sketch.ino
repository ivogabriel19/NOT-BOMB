/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos
 *                 Arduino UNO con display 7 segmentos CA
*/

#define a 11 //16
#define b 10 //05
#define c 8 //04
#define d 7 //00
#define e 13 //02
#define f 12 //14
#define g 9 //12
#define p 6 //13

#define INTERVAL 1000

const int pines[] = {a, b, c, d, e, f, g, p};
const bool common = true; // True = CA | False = CC
const int digitos[][7] = {
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,0,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}  //9
  };

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < (sizeof(pines)/sizeof(pines[0])) ; i++){
    pinMode(pines[i], OUTPUT);
    }

  for(int i = 0; i < (sizeof(pines)/sizeof(pines[0])) ; i++){
    digitalWrite(pines[i], HIGH);
    delay(300);
    digitalWrite(pines[i], LOW);
    }
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
  digitalWrite(p, LEDstate);
  LEDstate = !LEDstate;

  Serial.print("Numero " + String(num) + ": {");
  for (int pin=0 ; pin < (sizeof(pines)/sizeof(pines[0]))-1 ; pin++){
    digitalWrite(pines[pin], digitos[num][pin] ^ common);
    Serial.print(String(digitos[num][pines[pin]]) + ", ");
    }
  //for (int pin=0 ; pin < (sizeof(pines)/sizeof(pines[0]))-1 ; pin++){
  //    Serial.print(digitos[num][pin]);
  //  }
  Serial.println("}");
   
  num++;
  if(num>9) num=0;
  //Serial.println("blink_segundero() " + String(num));
  }
