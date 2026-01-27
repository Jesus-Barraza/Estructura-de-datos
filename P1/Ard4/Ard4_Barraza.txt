#include <DHT.h>
#define DHTPIN 23  //pin digital, sensor de señal
#define DHTTYPE DHT11 //modelo a usar lol

#define temp_limit 26
#define hum_limit 90

DHT dht(DHTPIN, DHTTYPE); //requisitos de la librería

const int N=10;
int indice=0;

float temper[N];
float hume[N];
bool antilag = false;

int LEDs[] = {4, 18};
int numLed = 2;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLed; i++) {
    pinMode(LEDs[i], OUTPUT);
  }

  Serial.begin(9600);
  Serial.print("Sensor test");
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  temper[indice] = temp;
  hume[indice] = hum; 

  indice+=1;

  if (indice==N) {
    indice = 0;
    antilag = true;
  }

  if (antilag) {
    float sum_temp=0;
    float sum_hum=0;

    for (int i = 0; i < N; i++) {
      sum_temp += temper[i];
      sum_hum += hume[i];

      temper[i] = 0;
      hume[i] = 0;
    }

    float total_temp = sum_temp/N;
    float total_hum = sum_hum/N;

    Serial.println("Full temperature level: ");
    Serial.println(total_temp);

    Serial.println("Full humidity level: ");
    Serial.println(total_hum);

    if (temp_limit < total_temp) {
      digitalWrite(LEDs[0], HIGH);
    } else {
      digitalWrite(LEDs[0], LOW);
    }

    if (hum_limit < total_hum) {
      digitalWrite(LEDs[1], HIGH);
    } else {
      digitalWrite(LEDs[1], LOW);
    }

    antilag=false;
  } else {
    Serial.println("Temperature level:");
    Serial.println(temper[indice-1]);

    Serial.println("Humidity level:");
    Serial.println(hume[indice-1]);
  }
  delay(2000);
}
