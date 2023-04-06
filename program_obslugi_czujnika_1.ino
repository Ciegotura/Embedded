//#include "TimerOne.h"    

#define OUT_SENSOR_1  2
volatile unsigned int licznik_czujnika_1=0; // licznik tikow

void counter() {
 static unsigned long lastTime;
  unsigned long timeNow = millis(); //eliminacja drgania stykow
  if (timeNow - lastTime < 1)
    return;

  licznik_czujnika_1++;
  lastTime = timeNow;
  
}

// the setup function runs once when you press reset or power the board

void setup() {
  

  Serial.begin(9600); //Ustawienie portu szeregowego
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUT_SENSOR_1, INPUT); 
  
  attachInterrupt(0,counter,RISING);// przerwanie od pinu 2 nr 0   SPRAWDZ TUTAJ MOZE SIE RECZNIE BEDZIE DALO ODCZYTAC LUB USAWIC ODPOWIEDNIE BITY W ATMEDZE
}
// the loop function runs over and over again forever
void loop() {
  bool Czujnik = digitalRead(OUT_SENSOR_1);
  Serial.println(Czujnik);
  Serial.println(licznik_czujnika_1);
  if(Czujnik){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
    }


}
