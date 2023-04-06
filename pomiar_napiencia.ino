float val;
float val_przeliczona;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(A0, INPUT);
}

void loop()
{
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000); // Wait for 1000 millisecond(s)
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(7,HIGH);
  val = analogRead(A0);
  val_przeliczona=5.0*val/1023;
  val = map(val, 0, 1023, 0, 500);
  Serial.print(val);
  Serial.println("Mapowane V");
  Serial.print(val_przeliczona);
  Serial.println("Przeliczone V");
  delay(1000);
}
