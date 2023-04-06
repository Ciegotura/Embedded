//Original source code : http://enrique.latorres.org/2017/10/17/testing-lolin-nodemcu-v3-esp8266/
//Download LoLin NodeMCU V3 ESP8266 Board for Arduino IDE (json) : http://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <ESP8266WiFi.h> // DOLONCZAM URZYWANOM BIBLIOTEKE W KODZIE
const char* ssid = "lapptoppik"; //NAZWA WIFI DO KTOREJ SIE LONCZYMY
const char* password = "felgi123lol1000"; //HASLO DO WIFI 
int ledPin = 2; // Arduino standard is GPIO13 but lolin nodeMCU is 2 http://www.esp8266.com/viewtopic.php?f=26&t=13410#p61332
WiFiServer server(80); //DEFINIUJE NUMER PORTU PO KTURYM MODEMCU BEDZIE SIE KOMUNIKOWAL
int jeden = LOW; //ZMIENNA DO TYCH DWUCH PRZYCISKOW CO DODALEM
void setup() {
 Serial.begin(115200); // USTAWIAM PRENDKOSC TRANSMISJI 
 delay(10);
 pinMode(ledPin, OUTPUT); //USTAWIANIE PINOW
 digitalWrite(ledPin, HIGH);
// Connect to WiFi network
Serial.println();
Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid); //PRINTUJE NA MONITOR PORTU SZEREGOWEGO 

WiFi.begin(ssid, password); //LONCZE SIE Z WIFI NA PODSTAWIE WPROWADZONYCH DANYCH 

while (WiFi.status() != WL_CONNECTED) { //SPRAWDZAM CZY POLONCZENIE SIE POWIODLO 
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");

// Start the server
 server.begin(); //URUCHAMIAM SERVER
 Serial.println("Server started");

// Print the IP address
 Serial.print("Use this URL to connect: ");
 Serial.print("http://");
 Serial.print(WiFi.localIP()); //POJAWIA SIE ADRES IP POLONCZENIA SIECIOWEGO 
 Serial.println("/");

}

void loop() {
 // Check if a client has connected
 WiFiClient client = server.available(); //NASLUCHIWANIE POLONCZEN OD KLIJENTOW
 if (!client) {
 return;
 }

// Wait until the client sends some data
 Serial.println("new client");
 while(!client.available()){
 delay(1);
 }

// Read the first line of the request
 String request = client.readStringUntil('r'); //ZMIENNA PRZECHOWUJACA RZONDANIE
 Serial.println(request);
 client.flush(); // FUNKCJA ODRZUCA WSZYSTKIE BAJTY WYSLANE PRZEZ KLIJENTA, KTORE NIE ZOSTALY ODCZYTANE

// Match the request
 //Lolin nodeMCU has inverted the LED.
 //Low level turns on the led
 //while High level turns it off

int value = HIGH; //initially off
 if (request.indexOf("/LED=OFF") != -1) {
 digitalWrite(ledPin, HIGH);
 value = HIGH;
 }
 if (request.indexOf("/LED=ON") != -1) {
 digitalWrite(ledPin, LOW);
 value = LOW;
 }
 //int jeden = LOW;
  if (request.indexOf("/1") != -1) {
  jeden = HIGH;
 } //REQUEST OD PRZYCISKU
 if (request.indexOf("/0") != -1) {
  jeden = LOW;
 }

// Set ledPin according to the request
 //digitalWrite(ledPin, value);

// Return the response
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println(""); 
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"); //Daje to przeglądarce instrukcje dotyczące kontrolowania wymiarów i skalowania strony.
 client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">"); //Wymagany atrybut rel określa relację między bieżącym dokumentem a połączonym dokumentem/zasobem.
 client.println("<style>p.padding{padding-left: 0.4cm;}p{color: black;}cred{color: red}cgreen{color: green}</style>"); //GRUPOWE FORMATOWANIE STYLU

 client.print("<br><p class=\"padding\">On-Board Led is now : ");     
 //High=off
 //Low=on

if(value == HIGH) {
 client.print("<cred>Off</cred>");
 } else {
 client.print("<cgreen>On<cgreen></p>");
 }
if(jeden == HIGH) {
 client.print("<cred>1</cred>");
 }else {
 client.print("<cgreen>0<cgreen></p>");
 }//WPISANIE WARTOSCI JEDEN DO NAPISU
 client.println("<div class=\"w3-container\">");
 client.println("<br>");
 client.println("<a href=\"/LED=ON\"\"><button class=\"w3-btn w3-ripple w3-green\">Turn On </button></a>");
 client.println("<a href=\"/LED=OFF\"\"><button class=\"w3-btn w3-ripple w3-red\">Turn Off </button></a><br />");
 client.println("<a href=\"/1\"\"><button class=\"w3-btn w3-ripple w3-yellow\">1 </button></a><br />"); //DODALEM SE PRZYCISK na request1?
 client.println("<a href=\"/0\"\"><button class=\"w3-btn w3-ripple w3-yellow\">0 </button></a><br />"); //DODALEM SE PRZYCISK na request1?
 client.println("</div>");
 client.println("</html>");

delay(1);
 Serial.println("Client disonnected");
 Serial.println("");
}
