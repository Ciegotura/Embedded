#include <ESP8266WiFi.h> 
const char* ssid = "Nazwa_WiFi";             
const char* password = "haslo";                        
WiFiServer server(80); 
int ledPin = 2; 
int czytaj = -1; 
unsigned int wynik = 0;
unsigned int *tab;
int *tab_bit; 
int licznik_wyslania = 0;
unsigned int indeks_rozkazu = 0;
int wyswietl = -1;
int rozmiar_tablicy_dynamicznej = 0; 
int stala_jeden = 1;
int value = HIGH; 
int licznik_odebrania1 = 0;
int licznik_odebrania2 = 0;
int *tab_silnik1;
int u = 2;
int tmp_uart_war = -1;
bool trajektoria_uart = true;
bool status_wykonania_ruchu = true;
short wykonanie_ruchu = -1;
unsigned int rozmiar_tablicy_trajektorii = 0;
int wynik_p;


int liczba_z_panelu_1(unsigned int wynik, int czytaj){
  wynik = wynik*10+ czytaj;
  return(wynik);
}

int przycisk_undo(unsigned int wynik){
  wynik = (wynik)/10;
  return(wynik);
}

void setup() {
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, HIGH);

  WiFi.begin(ssid, password); 

  server.begin(); 

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('r'); 

  client.flush(); 

  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, HIGH);
    value = HIGH;
    czytaj = 22;
  }
  if (request.indexOf("/LED=ON") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
    czytaj = 23;
  }
  if (request.indexOf("/0") != -1){
    czytaj = 0;
  }
  if (request.indexOf("/jeden") != -1){
    czytaj = 1;
  }
  if (request.indexOf("/2") != -1){
    czytaj = 2;
  }
  if (request.indexOf("/3") != -1){
    czytaj = 3;
  }
  if (request.indexOf("/4") != -1){
    czytaj = 4;
  }
  if (request.indexOf("/5") != -1){
    czytaj = 5;
  }
  if (request.indexOf("/6") != -1){
    czytaj = 6;
  }
  if (request.indexOf("/7") != -1){
    czytaj = 7;
  }
  if (request.indexOf("/8") != -1){
    czytaj = 8;
  }
  if (request.indexOf("/9") != -1){
    czytaj = 9;
  }
  if (request.indexOf("/c") != -1){
    licznik_wyslania = 0;
    czytaj = 10;
  }
  if (request.indexOf("/undo") != -1){
    czytaj = 11;
  }
  if (request.indexOf("/en") != -1){
    czytaj = 12;
  }
  if (request.indexOf("/en2") != -1){
    czytaj = 14; 
  }
  if (request.indexOf("/90p") != -1){
    czytaj = 16;
  }
  if (request.indexOf("/90l") != -1){
    czytaj = 17;
  }
  if (request.indexOf("/f") != -1){
    czytaj = 18;
  }
  if (request.indexOf("/back") != -1){
    czytaj = 19;
  }
  if (request.indexOf("/go") != -1){
    czytaj = 20; 
  }
  if (request.indexOf("/xd") != -1){
    czytaj = 21; 
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
  client.println("<style>cred{color: red}cgreen{color: green}"); 
  client.println("div.ekran{border-style: solid;width: 545px;margin-left: auto;margin-right: auto;margin-bottom: 3px;margin-top: 3px;border-radius: 4px;}"); 
  client.println("body{background-color: #26282E; font-size: 28px; color: #209781;text-align: center;font-family: 'Lobster', sans-serif;margin: 0;}");
  client.println("h1.heder {border-style: solid;border-radius: 4px;font-size: 64px;font-weight: 400;text-align: center;letter-spacing: 5px;margin-top: 20 px;margin-bottom: 0px;}");
  client.println("div.d{display: inline-block;}");
  client.println("div.przyciski_diody{border-style: solid;width: 545px;margin-left: auto;margin-right: auto;margin-bottom: 3px;border-radius: 4px;}");
  client.println("div.wynik{border-style: solid;margin-left: auto;margin-right: auto;margin-bottom: 3px;margin-top: 3px;border-radius: 4px;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<header><h1 class=\"heder\">Panel sterowania robotem</h1></header>");
  client.print("<br>");   


  switch (czytaj){   

    case 0:
      wynik = liczba_z_panelu_1(wynik,czytaj);   
    break;
    case 1:
      wynik = liczba_z_panelu_1(wynik,czytaj);  
    break;
    case 2:
      wynik = liczba_z_panelu_1(wynik,czytaj);   
    break;
    case 3:
      wynik = liczba_z_panelu_1(wynik,czytaj);   
    break;
    case 4:
      wynik = liczba_z_panelu_1(wynik,czytaj);   
    break;
    case 5:
      wynik = liczba_z_panelu_1(wynik,czytaj);   
    break;
    case 6:
      wynik = liczba_z_panelu_1(wynik,czytaj); 
    break;
    case 7:
      wynik = liczba_z_panelu_1(wynik,czytaj); 
    break;
    case 8:
      wynik = liczba_z_panelu_1(wynik,czytaj); 
    break;
    case 9:
      wynik = liczba_z_panelu_1(wynik,czytaj); 
    break;
    case 10:   przycisk c
      wynik = 0;                                                           
      indeks_rozkazu = 0;
      rozmiar_tablicy_dynamicznej=0;
      free(tab);
      free(tab_bit);   
      free(tab_silnik1);
      licznik_odebrania1 = 0;
      licznik_odebrania2 = 0;
      u = 2;
      tmp_uart_war = -1;
      trajektoria_uart = true;
      status_wykonania_ruchu = true;
      wykonanie_ruchu = -1;

    break;
    case 11: 
      wynik = przycisk_undo(wynik);
       przycisk undo
    break;
    case 12:                                                                  
    wynik_p=wynik/10;
    wynik_p=wynik_p*10;
    wynik_p=wynik-wynik_p;
      if(wynik_p==1||wynik_p==2||wynik_p==3||wynik_p==4){ 
        if(rozmiar_tablicy_dynamicznej!=0&&indeks_rozkazu<rozmiar_tablicy_dynamicznej){
          *(tab+indeks_rozkazu)=wynik;
          *(tab_bit+indeks_rozkazu)=wynik/256;                                         
          wynik = 0;
          indeks_rozkazu = indeks_rozkazu +1;     przycisk enter
          for(unsigned int i =0;i<indeks_rozkazu;i=i+1){
            wyswietl = *(tab+i);
          } 
        }
      }
      czytaj = 13; 
    break;
    case 14:
      if(rozmiar_tablicy_dynamicznej!=0){
        if(indeks_rozkazu>0){
          *(tab+indeks_rozkazu)=0;
          indeks_rozkazu = indeks_rozkazu -1;
          czytaj = 15;
        }
      } 
    break;
    case 16: //90p
      if(rozmiar_tablicy_dynamicznej!=0){
        wynik = liczba_z_panelu_1(wynik,3);
      }
    break;
    case 17: //90l
      if(rozmiar_tablicy_dynamicznej!=0){
        wynik = liczba_z_panelu_1(wynik,4);
      }
    break;
    case 18: //for
      if(rozmiar_tablicy_dynamicznej!=0){
        wynik = liczba_z_panelu_1(wynik,stala_jeden);
      }
    break;
    case 19: //back
      if(rozmiar_tablicy_dynamicznej!=0){
        wynik = liczba_z_panelu_1(wynik,2);
      }
    break;
    case 20:                             //Obsluga przycisku GO
      if(rozmiar_tablicy_dynamicznej!=0&&indeks_rozkazu==rozmiar_tablicy_dynamicznej){
        while(licznik_wyslania < rozmiar_tablicy_dynamicznej){
          if(Serial.availableForWrite()>sizeof(int)){
            Serial.write(tab_bit[licznik_wyslania]);
            Serial.flush();
            Serial.write(tab[licznik_wyslania]);
            Serial.flush();
            licznik_wyslania = licznik_wyslania + 1;
          }
        }
      }
    break;
    case 21:
      if(wynik!=0){
        rozmiar_tablicy_dynamicznej = wynik;                                        
        tab = (unsigned int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int)); 
        tab_bit = (int*)calloc(rozmiar_tablicy_dynamicznej,sizeof(int));
        wynik = 0;
        Serial.write(rozmiar_tablicy_dynamicznej);   
        Serial.flush();
      }
    break;
    case 22:
          
    break;
    case 23:
    
    break;
    default:
    
    break;
  }
  
  client.println("<div class=\"przyciski_diody\">");
  client.println("<div class=\"d\">Test diody: "); 
  if(value == HIGH){
    client.print("<cred>Off</cred>");
  }else{
    client.print("<cgreen>On</cgreen>");
  }
  client.println("</div>"); 
  client.println("<a href=\"/LED=ON\"\"><button class=\"w3-btn w3-ripple w3-green\" style=\"border-radius: 4px;\">On </button></a>"); 
  client.println("<a href=\"/LED=OFF\"\"><button class=\"w3-btn w3-ripple w3-red\" style=\"border-radius: 4px;\">Off </button></a>");
  
  client.println("</div>");
  client.print("<div class=\"ekran\">");  
  if(rozmiar_tablicy_dynamicznej!=0){
  client.print(rozmiar_tablicy_dynamicznej-indeks_rozkazu);}
  client.print(":");
  client.print(wynik);   
  client.println("</div>");
  
  client.println("<div class=\"w3-container\">");
  client.println("<a href=\"/7\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\" >7 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/8\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">8 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/9\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">9 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/90p\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">left </button></a><br />"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/4\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">4 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/5\"\"><button class=\"w3-btn w3-ripple w3-yellow \" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">5 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/6\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">6 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/90l\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">right </button></a><br />"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/jeden\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">1 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/2\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">2 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/3\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">3 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/f\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">forward </button></a><br />"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/undo\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">undo </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/0\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">0 </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/c\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">c </button></a>"); //DODALEM SE PRZYCISK na request1?   TAAAk
  client.println("<a href=\"/back\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">backward </button></a><br />"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/en\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">enter </button></a>"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/en2\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">unenter </button></a>");
  client.println("<a href=\"/go\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">go </button></a>"); //DODALEM SE PRZYCISK na request1?br />"); //DODALEM SE PRZYCISK na request1?
  client.println("<a href=\"/xd\"\"><button class=\"w3-btn w3-ripple w3-yellow\" style=\"width:125px;border: 4px solid #51c8b2;border-radius: 4px; margin: 3px;\">rozmiar </button></a>"); //DODALEM SE PRZYCISK na request1?br />"); //DODALEM SE PRZYCISK na request1?
  client.println("</div>");
  
  client.println("<div class=\"wynik\">");   
  client.print(":");
    for(unsigned int i =0;i<indeks_rozkazu;i=i+1){
      wyswietl = *(tab+i);
      client.println(" ");
      client.println(wyswietl); 
    }
  client.println("</div>");
  
  client.print("<br>");   
  client.println("<div class=\"wynik\">:");  
 
    
    if(trajektoria_uart==true&&status_wykonania_ruchu==true){   
        if(Serial.available() > 0){
            tmp_uart_war = Serial.read();
             if(tmp_uart_war!=(-1)){
                rozmiar_tablicy_trajektorii =tmp_uart_war;
                tab_silnik1 = (int*)calloc(rozmiar_tablicy_trajektorii*4,sizeof(int));
                
                trajektoria_uart=false;
                status_wykonania_ruchu = false;
            }

        }

    }
    if(trajektoria_uart==false&&status_wykonania_ruchu==false){   
    if(Serial.available() > 0){
      tmp_uart_war = Serial.read();
      if(tmp_uart_war!=(-1)){
        wykonanie_ruchu =tmp_uart_war;
            
        status_wykonania_ruchu=true; 
      
      }

    }

  }

    if(trajektoria_uart==false&&status_wykonania_ruchu==true){
        if(licznik_odebrania1<rozmiar_tablicy_trajektorii*2){
            while(Serial.available() > 0){
                tmp_uart_war = Serial.read();
                if(tmp_uart_war!=(-1)){
                    tab_silnik1[licznik_odebrania1] =tmp_uart_war;
                    licznik_odebrania1 = licznik_odebrania1 + 1; 
                }
            }
        }
  
    }

  
  if(licznik_odebrania1 == rozmiar_tablicy_trajektorii*4){
                                    
    for(int i = 0;i<rozmiar_tablicy_trajektorii*4;i=i+2){
      
      tab_silnik1[i]=tab_silnik1[i+1]+tab_silnik1[i]*256;
      licznik_odebrania1 = licznik_odebrania1+1; 
    }
  }
 if(licznik_odebrania1 > rozmiar_tablicy_trajektorii*4){
  int tmp = 0;
    for(int i = 0;i<rozmiar_tablicy_trajektorii;i++){
      client.println(" ");
      client.println(tab_silnik1[i+tmp]); 
      tmp=tmp+1;
    }
 }
    
  client.print("</div>");

  client.println("<div class=\"wynik\">:");   
  
  
 
  if(licznik_odebrania1 > rozmiar_tablicy_trajektorii*4){
  int tmp = 0;
    for(int i = 0;i<rozmiar_tablicy_trajektorii;i++){
      client.println(" ");
      client.println(tab_silnik1[i+tmp+rozmiar_tablicy_trajektorii*2]); 
      tmp=tmp+1;
    }
 }
  client.println("</div>");  
  
  client.println("<div class=\"ekran\">:");  
  

  if(wykonanie_ruchu==0){
    client.println("Wykonano");
  
  }else if(wykonanie_ruchu==1){
    client.println("Nie udalo sie");

  }
  client.println("</div>");  



    client.println("</body>");
  client.println("</html>");


}