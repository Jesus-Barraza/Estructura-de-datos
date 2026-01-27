#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define temp_limit 26
#define hum_limit 90

const int N = 10;
int indice = 0;

float temper[N];
float hume[N];
bool antilag = false;

int LEDs[] = {4, 18};
int numLed = 2;

#define boton 19
bool button = LOW;
bool lastButton = HIGH;
bool monitoreo = false;

/* 🔧 CONTROL DE TIEMPO (MÍNIMO) */
unsigned long tiempoAnterior = 0;
const unsigned long INTERVALO = 2000;

void setup() {
  for (int i = 0; i < numLed; i++) {
    pinMode(LEDs[i], OUTPUT);
    digitalWrite(LEDs[i], LOW);
  }

  pinMode(boton, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Sensor test");
  dht.begin();
}

void loop() {

  button = digitalRead(boton);

  // 🔧 Detección de pulsación (mínimo ajuste)
  if (lastButton == HIGH && button == LOW) {
    delay(30); // antirrebote simple
    monitoreo = !monitoreo;

    if (monitoreo) {
      Serial.println(">>> MONITOREO INICIADO <<<");
      tiempoAnterior = millis();
    } else {
      Serial.println(">>> MONITOREO DETENIDO <<<");

      for (int i = 0; i < numLed; i++) {
        digitalWrite(LEDs[i], LOW);
      }

      // 🔧 Reset correcto
      indice = 0;
      antilag = false;
    }
  }

  lastButton = button;

  // 🔧 Control de tiempo para el DHT
  if (monitoreo && millis() - tiempoAnterior >= INTERVALO) {
    tiempoAnterior = millis();

    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    // 🔧 Validación mínima
    if (isnan(hum) || isnan(temp)) {
      Serial.println("Error leyendo DHT");
      return;
    }

    temper[indice] = temp;
    hume[indice] = hum;
    indice++;

    if (indice == N) {
      indice = 0;
      antilag = true;
    }

    if (antilag) {
      float sum_temp = 0;
      float sum_hum = 0;

      for (int i = 0; i < N; i++) {
        sum_temp += temper[i];
        sum_hum += hume[i];
      }

      float total_temp = sum_temp / N;
      float total_hum = sum_hum / N;

      Serial.println("Full temperature level:");
      Serial.println(total_temp);

      Serial.println("Full humidity level:");
      Serial.println(total_hum);

      digitalWrite(LEDs[0], total_temp > temp_limit);
      digitalWrite(LEDs[1], total_hum > hum_limit);

      antilag = false;
    } else {
      Serial.println("Temperature level:");
      Serial.println(temp);

      Serial.println("Humidity level:");
      Serial.println(hum);
    }
  }
}
