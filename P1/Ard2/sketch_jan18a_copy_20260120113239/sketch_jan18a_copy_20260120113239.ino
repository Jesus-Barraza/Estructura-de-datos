const int LED1 = 2;
const int LED2 = 4;
const int LED3 = 5;

void setup() {
  // put your setup code here, to run once:
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LED1,HIGH);
delay(2000);
digitalWrite(LED1,LOW);

digitalWrite(LED2,HIGH);
delay(2000);
digitalWrite(LED2,LOW);

digitalWrite(LED3,HIGH);
delay(2000);
digitalWrite(LED3,LOW);
}