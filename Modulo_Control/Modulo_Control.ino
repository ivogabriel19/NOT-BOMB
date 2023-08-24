/* Proyecto para el scape room de captacion
 * First Approach: Temporizador en 7 segmentos de dos digitos
 *                 ESP32 DevKit
*/

#define a 16
#define b 17
#define c 18
#define d 19
#define t1 22
#define t2 23
#define seg 2 //Built in LED

#define INTERVAL 1000

const int pines[] = {d, c, b, a};
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
  pinMode(t1 ,OUTPUT);
  pinMode(t2 ,OUTPUT);

  digitalWrite(t1, LOW);
  digitalWrite(t2, LOW);
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
