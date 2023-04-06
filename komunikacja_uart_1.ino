const unsigned int MAX_MESSAGE_LENGTH = 12;
int rozkaz;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); //9.6k bitow na sekunde
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
 {
 static char message[MAX_MESSAGE_LENGTH];
 static unsigned int message_pos = 0;
   char inByte = Serial.read();
   if ( inByte != '\n'&& (message_pos < MAX_MESSAGE_LENGTH - 1))
{
message[message_pos] = inByte;
 message_pos++;
}
//Full message received...
else
{
message[message_pos] = '\0';

 //Print the message (or do other things)
 Serial.println(message);
int number = atoi(message);
rozkaz = number;
 Serial.println(number);
 //Reset for the next message
 message_pos = 0;
}
 }
}
