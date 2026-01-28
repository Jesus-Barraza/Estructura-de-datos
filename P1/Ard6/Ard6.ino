//Componentes del sensor de movimiento
#define PIRsensor 5

//Componentes del buzzer
#define Buzzer 22
const char melody[] = { "NOTE_C4" };
int noteDuration[] = {500};

//Componentes de los LEDs
int LEDs[] = {32, 33, 34, 35};
int numLeds = 4;

//Componentes de la pantalla

//Componentes del sensor de temperatura
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 

//componentes del boton
#define boton 21
bool button = LOW;
bool lastButton = HIGH;
bool monitoreo = false;

//componentes de la pantalla
#define SCL 2
#define SDA 4


void setup() {
  pinMode(PIRsensor, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(PIRsensor) == HIGH) {
    tone(Buzzer, melody[0], noteDuration[0]);
    noTone(Buzzer);
    Serial.println("Zona sucia");
  } else {
    Serial.println("Zona limpia");
  }
  delay(500);
}
