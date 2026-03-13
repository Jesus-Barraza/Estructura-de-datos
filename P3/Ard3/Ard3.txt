//Definiciones (porque aparentemente no necesito estructura yei)
int LEDs[] = {19, 21, 22, 23};
int numLeds = 4;
int BTNs[] = {4, 5};
int numBtns = 2;

int moreTimerA = 0;
int moreTimerB = 0;
int limitTimer = 8000;

bool semaforo = true;

int BTN = HIGH;
int preBtn = HIGH;

//Función de agregar timer
void addTimer(int &moreTimer) {
  moreTimer+=3000;
}

//Función que quita el timer a 0
void resetTimer(int &moreTimer) {
  moreTimer=0;
}

//Funciones del semáforo
void green(bool light) {
  int verde;
  int rojo;
  int antverde;
  int antrojo;
  if (light) {
    verde = LEDs[0];
    rojo = LEDs[1];
    antverde = LEDs[2];
    antrojo = LEDs[3];
  } else {
    verde = LEDs[2];
    rojo = LEDs[3];
    antverde = LEDs[0];
    antrojo = LEDs[1];
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(verde, LOW);
    delay(300);
    digitalWrite(verde, HIGH);
    delay(300);
  }

  digitalWrite(verde, LOW);
  digitalWrite(rojo, HIGH);
  digitalWrite(antverde, HIGH);
  digitalWrite(antrojo, LOW);

  delay(100);
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLeds; i++) {
    pinMode(LEDs[i], OUTPUT);
  }
  for (int i = 0; i < numBtns; i++) {
    pinMode(BTNs[i], INPUT_PULLUP);
  }

  digitalWrite(LEDs[0], HIGH);
  digitalWrite(LEDs[1], LOW);
  digitalWrite(LEDs[2], LOW);
  digitalWrite(LEDs[3], HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (semaforo) {
    BTN = digitalRead(BTNs[0]);
  } else {
    BTN = digitalRead(BTNs[1]);
  }

  static unsigned long Debounce = 0;
  if (BTN == LOW && preBtn == HIGH && millis() - Debounce > 200) {
    if (semaforo) {
      addTimer(moreTimerA);
    } else {
      addTimer(moreTimerB);
    }
    Debounce = millis();
  }

  preBtn = BTN;

  static unsigned long Timer = millis();
  if (semaforo) {
    if (millis() - Timer >= limitTimer + moreTimerA) {
      green(semaforo);
      semaforo = !semaforo;
      resetTimer(moreTimerA);
      Timer = millis();
    }
  } else {
    if (millis() - Timer >= limitTimer + moreTimerB) {
      green(semaforo);
      semaforo = !semaforo;
      resetTimer(moreTimerB);
      Timer = millis();
    }
  }
}
