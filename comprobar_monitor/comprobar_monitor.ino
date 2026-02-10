#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(18, 19); // ajusta si usas otros pines
  Serial.println("Escaneando I2C...");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Encontrado en 0x");
      Serial.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No se encontraron dispositivos");

  delay(5000);
}