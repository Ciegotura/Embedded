#define OUT1 13               //HIGH
#define OUT2 12               //LOW    to jedzie do tylu 
#define OUT3 8                //HIGH 
#define OUT4 7                //LOW    to jedzie do tylu 

volatile unsigned int licznik_czujnika_1=0; // licznik tikow
volatile unsigned int licznik_czujnika_2=0;

void jazda(int tab[]){


  
for(int i = 0;i<(sizeof(tab)/sizeof(int));){
 int wybor_ruchu;
 wybor_ruchu = tab[i] - ((tab[i]/10)*10);  //dekodowanie z tablicy
  int jak_daleko = tab[i]/10;
  switch(wybor_ruchu){

    case 1:  //for
      digitalWrite(OUT1, LOW);
      digitalWrite(OUT2, HIGH);
      digitalWrite(OUT3, LOW);
      digitalWrite(OUT4, HIGH);
      if(licznik_czujnika_1==jak_daleko&&licznik_czujnika_2==jak_daleko){
    licznik_czujnika_1=0; // licznik tikow
    licznik_czujnika_2=0;
    i=i+1;}
    break;
    case 2:  //back
      digitalWrite(OUT1, HIGH);
      digitalWrite(OUT2, LOW);
      digitalWrite(OUT3, HIGH);
      digitalWrite(OUT4, LOW);
      if(licznik_czujnika_1==jak_daleko&&licznik_czujnika_2==jak_daleko){
    licznik_czujnika_1=0; // licznik tikow
    licznik_czujnika_2=0;
    i=i+1;}
    break;
    case 3:  //p
      digitalWrite(OUT1, LOW);
      digitalWrite(OUT2, HIGH);
      digitalWrite(OUT3, HIGH);
      digitalWrite(OUT4, LOW);
      if(licznik_czujnika_1==jak_daleko&&licznik_czujnika_2==jak_daleko){
    licznik_czujnika_1=0; // licznik tikow
    licznik_czujnika_2=0;
    i=i+1;}
    break;
    case 4:  //l
      digitalWrite(OUT1, HIGH);
      digitalWrite(OUT2, LOW);
      digitalWrite(OUT3, LOW);
      digitalWrite(OUT4, HIGH);
      if(licznik_czujnika_1==jak_daleko&&licznik_czujnika_2==jak_daleko){
    licznik_czujnika_1=0; // licznik tikow
    licznik_czujnika_2=0;
    i=i+1;}
    break;
    default:
      digitalWrite(OUT1, LOW);
      digitalWrite(OUT2, LOW);
      digitalWrite(OUT3, LOW);
      digitalWrite(OUT4, LOW);
    break;
    }
  
  
  }  
 
  }




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
