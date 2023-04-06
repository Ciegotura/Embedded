#define OUT_SENSOR_1  2
#define OUT_SENSOR_2  3
#define OUT1 13               //HIGH
#define OUT2 12               //LOW    to jedzie do tylu 
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
unsigned int licznik_sekwencji = 0;
unsigned short sekwencja_wymijania = 1;
unsigned int licznik_przerwan = 0;
bool Czy_nastapi_omijanie = false;
volatile unsigned short l1 = 0;
volatile unsigned short l2 = 0; //zmienne do zliczenia sprawdzenia czy jest przerwanie
int CM;        //odległość w cm
long CZAS;
int rozmiar = 0;
int licznik_miejsca =0; //to jest miejsce gdzie nastompilo przerwanie
int m1,m2;
bool blok_przerwania = false;
unsigned int licz_i = 0;
unsigned int miejsce_licz_i = 0;
bool nie_da_sie_jechac = false;
int rozmiar_wys = 0;
//PID
int czas_probkowania = 50; //czas probkowania
float time ;
float kp=8; 
float ki=0.2; 
float kd=3100; 
float PID_p1, PID_i1, PID_d1, PID_total1;
float PID_p2, PID_i2, PID_d2, PID_total2;
float dystans = 0.0;
float blad_dystansu_krok_wczesniej1, blad_dystansu1, blad_dystansu_krok_wczesniej2, blad_dystansu2; //bo mam dwa silniki 


void counter1() { //funkcja do obslugi enkodera
 static unsigned long czas_wczesniej;
  unsigned long czas_teraz = millis(); //eliminacja drgania stykow
  if (czas_teraz - czas_wczesniej < 1)
    return;

  licznik_czujnika_1++;
  l1++;
  czas_wczesniej = czas_teraz;
  if(l1>=10){
  
    digitalWrite(Trig, LOW);        //ustawienie stanu niskiego na 2 uS - impuls inicjalizujacy 
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy 
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    CZAS = pulseIn(Echo, HIGH);
    CM = CZAS / 58;                //szerokość odbitego impulsu w uS podzielone przez 58 to odleglosc w cm 

    if(blok_przerwania==false&&nie_da_sie_jechac==false&&wybor_ruchu!=3&&wybor_ruchu!=4){ //tu dalem nie da sie  jechac
      if(CM<40){
        Czy_nastapi_omijanie = true;
        blok_przerwania = true;
        licznik_przerwan++;

      }
    }                                                          //tabmonit1[licznik_miejsca+1]!=0
    if(CM<40&&blok_przerwania==true&&nie_da_sie_jechac==false&&sekwencja_wymijania%2==0){

        nie_da_sie_jechac = true;
    
    }
    l1=0;
    l2=0;
  }  
}
void counter2() {    //funkcja do obslugi enkodera
 static unsigned long czas_wczesniej;
  unsigned long czas_teraz = millis(); //eliminacja drgania stykow
  if (czas_teraz - czas_wczesniej < 1)
    return;

  licznik_czujnika_2++;
  l2++;
  czas_wczesniej = czas_teraz;
  if(l2>=10){

    digitalWrite(Trig, LOW);        //ustawienie stanu wysokiego na 2 uS - impuls inicjalizujacy 
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy 
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    CZAS = pulseIn(Echo, HIGH);
    CM = CZAS / 58;                //szerokość odbitego impulsu w uS podzielone przez 58 to odleglosc w cm 

    if(blok_przerwania==false&&nie_da_sie_jechac==false&&wybor_ruchu!=3&&wybor_ruchu!=4){  //tu dalem nie da sie jechac
      if(CM<30){
        Czy_nastapi_omijanie=true;
        blok_przerwania = true;
        licznik_przerwan++;

      }
    }
     if(CM<40&&blok_przerwania==true&&nie_da_sie_jechac==false&&sekwencja_wymijania%2==0){ //dzienki temu wykryje tylko jak bedzie jechac prosto 
          

           nie_da_sie_jechac = true;


    
    }
    l1=0;
    l2=0;
  }
  
} 


void przerwanie(){
                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, LOW);
                  digitalWrite(OUT3, LOW);
                  digitalWrite(OUT4, LOW); 

                  PID_reset();
                  
                  millis_wczesniej = millis();
                  millis_teraz = millis();
                  do{
                    millis_teraz = millis();
                    if(millis_teraz - millis_wczesniej > 500) {
                      *(tabmonit1+licznik_miejsca)=(licznik_czujnika_1)*10+wybor_ruchu;
                      *(tabmonit2+licznik_miejsca)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
                      break;
                    }
                  }while(1);
                    m1 = licznik_czujnika_1;
                    m2 = licznik_czujnika_2;
                    if(m1<m2){jak_daleko=m1;}else{jak_daleko=m2;};

           
            licznik_czujnika_1=0; // licznik tikow
            licznik_czujnika_2=0;
            rozmiar = rozmiar_tablicy_dynamicznej+licznik_przerwan*8;
            tab_trajektorii_tmp = (int*)calloc(rozmiar,sizeof(int));
            tabtmp_wym2 = (int*)calloc(rozmiar,sizeof(int));
            tabtmp_wym1 = (int*)calloc(rozmiar,sizeof(int));

            for(int e = 0;e<=licznik_miejsca;e++){
                *(tab_trajektorii_tmp+e)=*(tab_trajektorii+e);
                *(tabtmp_wym1+e)=*(tabmonit1+e);
                *(tabtmp_wym2+e)=*(tabmonit2+e);

            }

            tabmonit1=(int*)realloc(tabmonit1,(rozmiar*sizeof(int)));
            tabmonit2=(int*)realloc(tabmonit2,(rozmiar*sizeof(int)));
            tab1_bit=(int*)realloc(tab1_bit,(rozmiar*sizeof(int)));
            tab2_bit=(int*)realloc(tab2_bit,(rozmiar*sizeof(int)));

            for(int e = 0;e<=licznik_miejsca;e++){
              
                *(tabmonit1+e)=*(tabtmp_wym1+e);
                *(tabmonit2+e)=*(tabtmp_wym2+e);

            }

            free(tabtmp_wym1);
            free(tabtmp_wym2);

            for(int e=(licznik_miejsca+8+1);e<(rozmiar_tablicy_dynamicznej+licznik_przerwan*8);e++){
                *(tab_trajektorii_tmp+e)=*(tab_trajektorii+(e-8));
            }
            *(tab_trajektorii_tmp+(licznik_miejsca+1))=213;
            *(tab_trajektorii_tmp+(licznik_miejsca+2))=701;
            *(tab_trajektorii_tmp+(licznik_miejsca+3))=214;
            *(tab_trajektorii_tmp+(licznik_miejsca+4))=1401;
            *(tab_trajektorii_tmp+(licznik_miejsca+5))=214;
            *(tab_trajektorii_tmp+(licznik_miejsca+6))=701;
            *(tab_trajektorii_tmp+(licznik_miejsca+7))=213;
            free(tab_trajektorii);
            tab_trajektorii = (int*)calloc(rozmiar,sizeof(int));
            for(int e=0;e<(rozmiar_tablicy_dynamicznej+licznik_przerwan*8);e++){
                *(tab_trajektorii+e)=*(tab_trajektorii_tmp+e);
                if(tab_trajektorii[e]==0){
                    if(m1<m2){

                *(tab_trajektorii+e)=((*(tab_trajektorii+licznik_miejsca))/10)-m1;
                *(tab_trajektorii+e)=(*(tab_trajektorii+e))*10+wybor_ruchu;
            }else{

                *(tab_trajektorii+e)=((*(tab_trajektorii+licznik_miejsca))/10)-m2;
                *(tab_trajektorii+e)=(*(tab_trajektorii+e))*10+wybor_ruchu;
            }    
                    
                    
                }
            }
            free(tab_trajektorii_tmp);
            licz_i = licznik_miejsca;
            miejsce_licz_i = licz_i;
            sekwencja_wymijania = 1;
            
  }

void PID(){
  if (millis() > time+czas_probkowania){   

    time = millis();    
    dystans = jak_daleko*0.518;   
    blad_dystansu1 = dystans - licznik_czujnika_1*0.518;  
    blad_dystansu2 = dystans - licznik_czujnika_2*0.518;   
    PID_p1 = kp * blad_dystansu1;   
    PID_p2 = kp * blad_dystansu2;  

    PID_d1 = kd*(blad_dystansu1 - blad_dystansu_krok_wczesniej1);   
    PID_d2 = kd*(blad_dystansu2 - blad_dystansu_krok_wczesniej2);  
                  
    if(blad_dystansu1 < 10){
      PID_i1 = PID_i1 + (ki * blad_dystansu1);  
    }
    else{
      PID_i1 = 0;
    }

    if(blad_dystansu2 < 10){
      PID_i2 = PID_i2 + (ki * blad_dystansu2);  
    }
    else{
      PID_i2 = 0;
    }
  
    PID_total1 = PID_p1 + PID_i1 + PID_d1;
    PID_total2 = PID_p2 + PID_i2 + PID_d2;  
  
    if(PID_total1 < 60){PID_total1 = 60;}
    if(PID_total1 > 255) {PID_total1 = 255; } 

    if(PID_total2 < 60){PID_total2 = 60;}
    if(PID_total2 > 255) {PID_total2 = 255; }  
  
    analogWrite(9, PID_total1);  //prawy 2?
    analogWrite(10, PID_total2); //lewy 1?
    blad_dystansu_krok_wczesniej1 = blad_dystansu1;
    blad_dystansu_krok_wczesniej2 = blad_dystansu2;
  }
  
}

void PID_reset(){

  PID_p1=0; PID_i1=0; PID_d1=0; PID_total1=0;
  PID_p2=0; PID_i2=0; PID_d2=0; PID_total2=0;
  dystans = 0.0;
  blad_dystansu_krok_wczesniej1=0; 
  blad_dystansu1=0; 
  blad_dystansu_krok_wczesniej2=0; 
  blad_dystansu2=0; 

  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //9.6k bitow na sekunde
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUT_SENSOR_1, INPUT); 
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  attachInterrupt(0,counter1,RISING);// przerwanie od pinu 2 nr 0   SPRAWDZ TUTAJ MOZE SIE RECZNIE BEDZIE DALO ODCZYTAC LUB USAWIC ODPOWIEDNIE BITY W ATMEDZE
  attachInterrupt(1,counter2,RISING);// przerwanie od pinu 3 nr 1   SPRAWDZ TUTAJ MOZE SIE RECZNIE BEDZIE DALO ODCZYTAC LUB USAWIC ODPOWIEDNIE BITY W ATMEDZE
  pinMode(Trig, OUTPUT);                     //ustawienie pinu 4 w Arduino jako wyjście
  pinMode(Echo, INPUT);                      //ustawienie pinu 5 w Arduino jako wejście
  time = millis();
  analogWrite(9, 127);  //prawy 2?
  analogWrite(10, 127); //lewy 1?
}

void loop() {

 
  if(Serial.available() > 0){
    
      if(rozmiar_tablicy_dynamicznej == (-1)){
        rozmiar_tablicy_dynamicznej = Serial.read();
        nie_da_sie_jechac = false;                       //tu dalem zerowanie sie nie da sie jechac
        
        tab_trajektorii = (int*)calloc(rozmiar_tablicy_dynamicznej*2,sizeof(int)); //trajektoria
        tabmonit1 = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //zapis trajektorii silnik1
        tabmonit2 = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //zapis trajektorii silnik2
        tab1_bit = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //do wyslania zapisu z silnika1
        tab2_bit = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); //do wyslania zapisu z silnika2
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
  
  
 

 rozmiar=rozmiar_tablicy_dynamicznej;
    if(czy_mozna_jechac == true){
      for(int i = 0 ;i<rozmiar;){  //Jazda z tablicy TU DODALEM +1 ZEBY JECHALO TEZ Z JEDNYM RUCHEM 

        wybor_ruchu = tab_trajektorii[i] - ((tab_trajektorii[i]/10)*10);  //dekodowanie z tablicy     TO W PRZYPADKU KOMUNIKACJI POPRZEZ TABLICE
        jak_daleko = tab_trajektorii[i]/10;                                      //    TO W PRZYPADKU KOMUNIKACJI POPRZEZ TABLICE

        millis_wczesniej = millis();
        switch(wybor_ruchu){
          case 1:
            licznik_miejsca = i;  //for
            millis_wczesniej = millis();
            millis_teraz = millis();

            do{

              millis_teraz = millis();
              if(millis_teraz - millis_wczesniej > 500) {
                digitalWrite(OUT1, LOW);
                digitalWrite(OUT2, HIGH);
                digitalWrite(OUT3, LOW);
                digitalWrite(OUT4, HIGH);

                PID();
            
                if(Czy_nastapi_omijanie==true){
            przerwanie();
            Czy_nastapi_omijanie=false;
            i=i+1;
                break;
                }else if(nie_da_sie_jechac==true){

                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, LOW);
                  digitalWrite(OUT3, LOW);
                  digitalWrite(OUT4, LOW); 

                  PID_reset();
                  
                  millis_wczesniej = millis();
                  millis_teraz = millis();
                  do{
                    millis_teraz = millis();
                    if(millis_teraz - millis_wczesniej > 500) {

                      *(tabmonit1+i+licznik_sekwencji)=(licznik_czujnika_1)*10+wybor_ruchu;
                      *(tabmonit2+i+licznik_sekwencji)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
                      break;
                    }
                  }while(1);
                  rozmiar = i;
                  licznik_czujnika_1=0; // licznik tikow
                  licznik_czujnika_2=0;
                  break;

                  

                }
              }
            }while(licznik_czujnika_1<=jak_daleko&&licznik_czujnika_2<=jak_daleko);
          break;
          case 2:
            licznik_miejsca = i;  //back
            millis_wczesniej = millis();
            millis_teraz = millis();
            do{
              millis_teraz = millis();
              if(millis_teraz - millis_wczesniej > 500) {
      
                digitalWrite(OUT1, HIGH);
                digitalWrite(OUT2, LOW);
                digitalWrite(OUT3, HIGH);
                digitalWrite(OUT4, LOW);

                PID();
                
               if(Czy_nastapi_omijanie==true){
            przerwanie();
            Czy_nastapi_omijanie=false;
            i=i+1;
                    break;
                }else if(nie_da_sie_jechac==true){

                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, LOW);
                  digitalWrite(OUT3, LOW);
                  digitalWrite(OUT4, LOW); 

                  PID_reset();
                  
                  millis_wczesniej = millis();
                  millis_teraz = millis();
                  do{
                    millis_teraz = millis();
                    if(millis_teraz - millis_wczesniej > 500) {

                      *(tabmonit1+i+licznik_sekwencji)=(licznik_czujnika_1)*10+wybor_ruchu;
                      *(tabmonit2+i+licznik_sekwencji)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
                      break;
                    }
                  }while(1);

                  rozmiar = i;
                  licznik_czujnika_1=0; // licznik tikow
                  licznik_czujnika_2=0;
                  break;

                  

                }
              }

            }while(licznik_czujnika_1<=jak_daleko&&licznik_czujnika_2<=jak_daleko);
          break;
          case 3:

            licznik_miejsca = i;  
            millis_wczesniej = millis();
            millis_teraz = millis();
            do{
              millis_teraz = millis();
              if(millis_teraz - millis_wczesniej > 500) {
      
                digitalWrite(OUT1, LOW);
                digitalWrite(OUT2, HIGH);
                digitalWrite(OUT3, HIGH);
                digitalWrite(OUT4, LOW);

                PID();
                
               if(Czy_nastapi_omijanie==true){
            przerwanie();
            Czy_nastapi_omijanie=false;
            i=i+1;
            break;
                }else if(nie_da_sie_jechac==true){

                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, LOW);
                  digitalWrite(OUT3, LOW);
                  digitalWrite(OUT4, LOW); 

                  PID_reset();
                  
                  millis_wczesniej = millis();
                  millis_teraz = millis();
                  do{
                    millis_teraz = millis();
                    if(millis_teraz - millis_wczesniej > 500) {

                      *(tabmonit1+i+licznik_sekwencji)=(licznik_czujnika_1)*10+wybor_ruchu;
                      *(tabmonit2+i+licznik_sekwencji)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
                      break;
                    }
                  }while(1);
                  rozmiar = i;
                  licznik_czujnika_1=0; // licznik tikow
                  licznik_czujnika_2=0;
                  break;

                  

                }
  
              }
            }while(licznik_czujnika_1<=jak_daleko&&licznik_czujnika_2<=jak_daleko);
          break;
          case 4:

            licznik_miejsca = i; 
            millis_wczesniej = millis();
            millis_teraz = millis();
            do{
              millis_teraz = millis();
              if(millis_teraz - millis_wczesniej > 500) {
      
                digitalWrite(OUT1, HIGH);
                digitalWrite(OUT2, LOW);
                digitalWrite(OUT3, LOW);
                digitalWrite(OUT4, HIGH);

                PID();

                if(Czy_nastapi_omijanie==true){
            przerwanie();
            Czy_nastapi_omijanie=false;
            i=i+1;
               break;
                }else if(nie_da_sie_jechac==true){

                  //Serial.println("Nie da sie jechac_pierwsze");
                  digitalWrite(OUT1, LOW);
                  digitalWrite(OUT2, LOW);
                  digitalWrite(OUT3, LOW);
                  digitalWrite(OUT4, LOW); 

                  PID_reset();
                  
                  millis_wczesniej = millis();
                  millis_teraz = millis();
                  do{
                    millis_teraz = millis();
                    if(millis_teraz - millis_wczesniej > 500) {

                      *(tabmonit1+i+licznik_sekwencji)=(licznik_czujnika_1)*10+wybor_ruchu;
                      *(tabmonit2+i+licznik_sekwencji)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
                      break;
                    }
                  }while(1);

                  rozmiar = i;
                  licznik_czujnika_1=0; // licznik tikow
                  licznik_czujnika_2=0;
                  break;

                  

                }
              }
            }while(licznik_czujnika_1<=jak_daleko&&licznik_czujnika_2<=jak_daleko);
          break;
          default:
            digitalWrite(OUT1, LOW);
            digitalWrite(OUT2, LOW);
            digitalWrite(OUT3, LOW);
            digitalWrite(OUT4, LOW);
          break;
        }//koniec switcha
        //} //koniec ifa czy_nastapi_omijanie == false

       


        if(licznik_czujnika_1>=jak_daleko&&licznik_czujnika_2>=jak_daleko){

          digitalWrite(OUT1, LOW);
          digitalWrite(OUT2, LOW);
          digitalWrite(OUT3, LOW);
          digitalWrite(OUT4, LOW);  

          PID_reset();        

           if(blok_przerwania==true){
            licz_i++;
            sekwencja_wymijania++;
            l1=0;
            l2=0; //rzeby w kazdej sekwencji naliczylo 10 wystompien
            if(sekwencja_wymijania>=8){sekwencja_wymijania=0;}
         
            if(licz_i==(miejsce_licz_i+6)){blok_przerwania=false;}
           }
            millis_wczesniej = millis();
            millis_teraz = millis();
          do{
            millis_teraz = millis();
            if(millis_teraz - millis_wczesniej > 500) {

              *(tabmonit1+i+licznik_sekwencji)=(licznik_czujnika_1)*10+wybor_ruchu;
              *(tabmonit2+i+licznik_sekwencji)=(licznik_czujnika_2)*10+wybor_ruchu; //zapisanie do tablic monitorujacych 
              break; 
            }
          }while(1);

          licznik_czujnika_1=0; // licznik tikow
          licznik_czujnika_2=0;

          i=i+1;
        }

      
        if(i==(rozmiar_tablicy_dynamicznej+licznik_przerwan*8)&&nie_da_sie_jechac==false){ //ta pentla sie wykona raz 
          free(tab_trajektorii); //zwalnianie niejsca                          

          rozmiar = rozmiar_tablicy_dynamicznej+licznik_przerwan*8;

          Serial.write(rozmiar);
          Serial.flush();
          Serial.write(0); //wyslanie wartosci 1 czyli ze sie nie udalo
          Serial.flush();
 
         for(int t = 0;t<(rozmiar_tablicy_dynamicznej+licznik_przerwan*8);t++){    //Wyzlanie trajektorii silnika1
            *(tab1_bit+t)=tabmonit1[t]/256;

            Serial.write(tab1_bit[t]);
         
            Serial.flush();
        
            Serial.write(tabmonit1[t]);
       
            Serial.flush();
      
          }
          for(int t = 0;t<(rozmiar_tablicy_dynamicznej+licznik_przerwan*8);t++){      //Wyslanie trajektorii silnika2
            *(tab2_bit+t)=tabmonit2[t]/256;

            Serial.write(tab2_bit[t]);
  
            Serial.flush();
    
            Serial.write(tabmonit2[t]);
    
            Serial.flush();
    
          }

          
          rozmiar_tablicy_dynamicznej = -1;   
          free(tab1_bit);
          free(tab2_bit);
          free(tabmonit1);
          free(tabmonit2);
          czy_mozna_jechac = false; 
          licznik_przerwan = 0;
          Czy_nastapi_omijanie==false; //zakomentuj to tero
          l1 = 0;
          l2 = 0;
          blok_przerwania = false;
          nie_da_sie_jechac = false;

        
        }else if(nie_da_sie_jechac==true){

          free(tab_trajektorii); //zwalnianie niejsca                          //RESET TABLICY, W DALSZEJ CZENSCI PROGRAMU PRAWDOPODOBNIE TRZEBA GO BENDZIE GDZIE INDZIEJ DAC!!!!

          rozmiar_wys = i+1;

          Serial.write(rozmiar_wys);
          Serial.flush();
          Serial.write(1); //wyslanie wartosci 1 czyli ze sie nie udalo
          Serial.flush();
          for(int t = 0;t<rozmiar_wys;t++){    //Wyzlanie trajektorii silnika1
            *(tab1_bit+t)=tabmonit1[t]/256;

            Serial.write(tab1_bit[t]);
         
            Serial.flush();
        
            Serial.write(tabmonit1[t]);
       
            Serial.flush();
      
          }
          for(int t = 0;t<rozmiar_wys;t++){      //Wyslanie trajektorii silnika2
            *(tab2_bit+t)=tabmonit2[t]/256;

            Serial.write(tab2_bit[t]);
  
            Serial.flush();
    
            Serial.write(tabmonit2[t]);
    
            Serial.flush();
    
          }
          
          
          rozmiar_tablicy_dynamicznej = -1;   
          free(tab1_bit);
          free(tab2_bit);
          free(tabmonit1);
          free(tabmonit2);
          czy_mozna_jechac = false; 
          licznik_przerwan = 0;
          Czy_nastapi_omijanie==false; //zakomentuj to tero
          l1 = 0;
          l2 = 0;
          blok_przerwania = false;

          sekwencja_wymijania = 1;
        }  
        
        
        //koniec pentyli wykonujacej sie raz 
      }//koniec fora   
  
    }//koniec czy mozna jechac
 
}//koniec loop()
          