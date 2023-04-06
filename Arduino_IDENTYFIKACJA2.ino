#define OUT_SENSOR_1  2
#define OUT_SENSOR_2  3
#define OUT1 13               //HIGH
#define OUT2 12               //LOW    to jedzie do tylu to jest silnik prawy
#define OUT3 8                //HIGH 
#define OUT4 7                //LOW     to jedzie do tylu
#define Trig 4 //pin 4 Arduino połączony z pinem Trigger czujnika
#define Echo 5 //pin 5 Arduino połączony z pinem Echo czujnika

volatile unsigned int licznik_czujnika_1=0; // licznik tikow
volatile unsigned int licznik_czujnika_2=0; // licznik tikow
int *tab_trajektorii;
int *tabmonit1;    //tablica do monitorowania trajektorii silnik 1
int *tabmonit2;    //tablica do monitorowania trajektorii silnik 2
int *tab1_bit;
int *tab2_bit;
int tmp_uart_war =-1 ;   //Wartosc tymczasowa uart 
int licznik_tablicy = 0;               //licznik i tablica testowa do testu zapisywania tablicy 
int licznik_programu = 2; //zmienna zezwalajanca na jazde, jesli = 1 to jazda
int u = 2;    //ZMIENNA DO FORA SORTUJONCEGO TABLICE dYN
int rozmiar_tablicy_dynamicznej = -1;  //ZMIENNA POTRZEBNA DO DEKLARACJI TTABLICY DYNAMICZNEJ
bool czy_mozna_jechac = false;
int wybor_ruchu;
int jak_daleko;
long millis_wczesniej = 0; 
unsigned long millis_teraz = 0;
//Zmienne do przerwania
int *tabtmp_wym1;
int *tabtmp_wym2;
int *tab_trajektorii_tmp;
int rozmiar_wys = 0;
//PID

float time ;
unsigned int nr_probki=0;
unsigned int droga_wczesniej=0;
int czas_p = 50;
int tik[150]={0};



void counter1() { //funkcja do obslugi enkodera
 static unsigned long czas_wczesniej;
  unsigned long czas_teraz = millis(); //eliminacja drgania stykow
  if (czas_teraz - czas_wczesniej < 1)
    return;

  licznik_czujnika_1++;
   czas_wczesniej = czas_teraz;
 //Serial.println("przerwanie 1");
 
}
void counter2() {    //funkcja do obslugi enkodera
 static unsigned long czas_wczesniej;
  unsigned long czas_teraz = millis(); //eliminacja drgania stykow
  if (czas_teraz - czas_wczesniej < 1)
    return;

  licznik_czujnika_2++;
 czas_wczesniej = czas_teraz;
 //Serial.println("przerwanie 2");
} 
void rejestr(){
  if (millis() > time+czas_p&&nr_probki<150){   //tutaj co 50 ms jest wykonywana pentla

    time = millis();    
  
  tik[nr_probki]=(licznik_czujnika_2-droga_wczesniej);//tik[nr_probki]=(licznik_czujnika_1-droga_wczesniej)*0.518/(0.05);
  
  droga_wczesniej=licznik_czujnika_2;

  
  nr_probki++;}
  }






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //9.6k bitow na sekunde
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUT_SENSOR_1, INPUT); 
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  attachInterrupt(0,counter1,RISING);// przerwanie od pinu 2 nr 0   SPRAWDZ TUTAJ MOZE SIE RECZNIE BEDZIE DALO ODCZYTAC LUB USAWIC ODPOWIEDNIE BITY W ATMEDZE silnik prawy
  attachInterrupt(1,counter2,RISING);// przerwanie od pinu 3 nr 1   SPRAWDZ TUTAJ MOZE SIE RECZNIE BEDZIE DALO ODCZYTAC LUB USAWIC ODPOWIEDNIE BITY W ATMEDZE
  pinMode(Trig, OUTPUT);                     //ustawienie pinu 4 w Arduino jako wyjście
  pinMode(Echo, INPUT);                      //ustawienie pinu 5 w Arduino jako wejście
  time = millis();
  analogWrite(9, 255);  //prawy 2?
  analogWrite(10, 255); //lewy 1?
}

void loop() {


 
  if(Serial.available() > 0){
    
      if(rozmiar_tablicy_dynamicznej == (-1)){
        rozmiar_tablicy_dynamicznej = Serial.read();
     //Serial.println(rozmiar_tablicy_dynamicznej);    //TUUUUUUUUUUUUUUUUUUUUUUUUUUUU SSSSSSSSSSSSSSSSSSEEEEEEEEEEEEEEERRRRRRRRRRRIIIIIIIIIIAAAAAAAALLLLLLL PPPPPRRIIINNNTLLLLLN
        
        tab_trajektorii = (int*)calloc(rozmiar_tablicy_dynamicznej*2,sizeof(int)); //trajektoria
        //tabmonit1 = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //zapis trajektorii silnik1
        //tabmonit2 = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //zapis trajektorii silnik2
        //tab1_bit = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //do wyslania zapisu z silnika1
        //tab2_bit = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //do wyslania zapisu z silnika2
       }
  
        tmp_uart_war = Serial.read();
          if(rozmiar_tablicy_dynamicznej!=(-1)&&tmp_uart_war!=(-1)){
          tab_trajektorii[licznik_tablicy] = tmp_uart_war;
          licznik_tablicy = licznik_tablicy + 1;}
  }

 
  if(licznik_tablicy == rozmiar_tablicy_dynamicznej*2){
                                    //PRZELICZANIE WARTOSCI Z UARTA
    for(int i = 0;i<rozmiar_tablicy_dynamicznej*2;i=i+2){
      
      tab_trajektorii[i]=tab_trajektorii[i+1]+tab_trajektorii[i]*256;
    
    }
  
    for(int i = 0;i<rozmiar_tablicy_dynamicznej;i++){
  
    tab_trajektorii[i+1]=tab_trajektorii[u];
      u = u + 2;
    }
      licznik_tablicy = 0;
      u = 2;
                       //RESET TABLICY, W DALSZEJ CZENSCI PROGRAMU PRAWDOPODOBNIE TRZEBA GO BENDZIE GDZIE INDZIEJ DAC!!!!
      czy_mozna_jechac = true;     //    TO DO USUNIENCIA PRAWDOPODOBNIE
  }
  
  
 

  
    if(czy_mozna_jechac == true){

      for(int i = 0 ;i<rozmiar_tablicy_dynamicznej;){  //Jazda z tablicy TU DODALEM +1 ZEBY JECHALO TEZ Z JEDNYM RUCHEM 

        wybor_ruchu = tab_trajektorii[i] - ((tab_trajektorii[i]/10)*10);  //dekodowanie z tablicy     TO W PRZYPADKU KOMUNIKACJI POPRZEZ TABLICE
        jak_daleko = tab_trajektorii[i]/10;                                      //    TO W PRZYPADKU KOMUNIKACJI POPRZEZ TABLICE

     
        millis_wczesniej = millis();
        switch(wybor_ruchu){
          case 1:

            millis_wczesniej = millis();
            millis_teraz = millis();

            do{

              millis_teraz = millis();
              if(millis_teraz - millis_wczesniej > 500) {
                
                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, HIGH);
                  rejestr(); //testowanie silnika lewego
            
              
                digitalWrite(OUT3, LOW);
                digitalWrite(OUT4, HIGH);
              //Serial.println(licznik_czujnika_2);

               
              }
            }while(licznik_czujnika_2<=jak_daleko);
          break;
         
          default:
            digitalWrite(OUT1, LOW);
            digitalWrite(OUT2, LOW);
            digitalWrite(OUT3, LOW);
            digitalWrite(OUT4, LOW);
          break;
        }//koniec switcha
        //} //koniec ifa czy_nastapi_omijanie == false

       


        if(licznik_czujnika_2>=jak_daleko){

          digitalWrite(OUT1, LOW);
          digitalWrite(OUT2, LOW);
          digitalWrite(OUT3, LOW);
          digitalWrite(OUT4, LOW);  
     
            millis_wczesniej = millis();
            millis_teraz = millis();
          do{
            millis_teraz = millis();
            if(millis_teraz - millis_wczesniej > 500) {
              

              *(tabmonit1+i)=(licznik_czujnika_1)*10+wybor_ruchu;
              *(tabmonit2+i)=(licznik_czujnika_1)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
              break; 
            }
          }while(1);

          licznik_czujnika_1=0; // licznik tikow
          licznik_czujnika_2=0;
          i=i+1;
        }

      
        if(i==rozmiar_tablicy_dynamicznej){ //ta pentla sie wykona raz 
          free(tab_trajektorii); //zwalnianie niejsca                          

 
         for(int t = 0;t<(150);t++){    //Wyzlanie trajektorii silnika1
            //*(tab1_bit+t)=tabmonit1[t]/256;

            Serial.write(tik[t]);
         
            Serial.flush();
        
           // Serial.write(tabmonit1[t]);
       
            //Serial.flush();
      
          }
          //for(int t = 0;t<(rozmiar_tablicy_dynamicznej);t++){      //Wyslanie trajektorii silnika2
            //*(tab2_bit+t)=tabmonit2[t]/256;

           // Serial.write(tab2_bit[t]);
  
            //Serial.flush();
    
            //Serial.write(tabmonit2[t]);
    
            //Serial.flush();
    
          //}

          
          rozmiar_tablicy_dynamicznej = -1;   
          //free(tab1_bit);
          //free(tab2_bit);
          //free(tabmonit1);
          //free(tabmonit2);
          czy_mozna_jechac = false; 


        
        }  
        
        
        //koniec pentyli wykonujacej sie raz 
      }//koniec fora   
  
    }//koniec czy mozna jechac
 
}//koniec loop()
     
