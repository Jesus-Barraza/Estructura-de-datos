
//  PRUEBA DE MOTORES - ESP32 + Puente H L298N
//  Motor A: IN1 -> D27 | IN2 -> D26
//  Motor B: IN3 -> D25 | IN4 -> D33

// --- Pines Motor A ---
// modificar segun conexión con ESP32
#define IN1 27
#define IN2 26

// --- Pines Motor B ---
// modificar segun conexión con ESP32
#define IN3 25
#define IN4 33

// --- Velocidad PWM (0-255) ---
#define VELOCIDAD 200

//  Funciones Motor A
void motorA_adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void motorA_atras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void motorA_stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

//  Funciones Motor B
void motorB_adelante() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void motorB_atras() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void motorB_stop() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//  Funciones combinadas
void ambos_adelante() {
  motorA_adelante();
  motorB_adelante();
}

void ambos_atras() {
  motorA_atras();
  motorB_atras();
}

void ambos_stop() {
  motorA_stop();
  motorB_stop();
}

void girar_derecha() {
  motorA_adelante();
  motorB_atras();
}

void girar_izquierda() {
  motorA_atras();
  motorB_adelante();
}


void setup() {
  Serial.begin(115200);

  // Configurar pines como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Asegurarse de que los motores empiecen detenidos
  ambos_stop();

  Serial.println("=== PRUEBA DE MOTORES ESP32 + L298N ===");
  Serial.println("Iniciando secuencia de prueba...");
  delay(2000);
}


void loop() {
// Demostración automatica, el carrito hará todo autónomo al arrancar
  // --- 1. Ambos motores ADELANTE ---
  ambos_adelante();
  delay(2000);

  // --- STOP ---
  ambos_stop();
  delay(500);

  // --- 2. Ambos motores ATRÁS ---
  ambos_atras();
  delay(2000);

  // --- STOP ---
  ambos_stop();
  delay(500);

  // --- 3. GIRAR A LA DERECHA ---
  girar_derecha();
  delay(1500);

  // --- STOP ---
  ambos_stop();
  delay(500);

  // --- 4. GIRAR A LA IZQUIERDA ---
  girar_izquierda();
  delay(1500);

  // --- STOP ---
  ambos_stop();
  delay(500);

  // --- 5. Prueba Motor A solo ---
  motorA_adelante();
  delay(1000);
  motorA_stop();
  delay(300);

  motorA_atras();
  delay(1000);
  motorA_stop();
  delay(300);

  // --- 6. Prueba Motor B solo ---
  motorB_adelante();
  delay(1000);
  motorB_stop();
  delay(300);

  motorB_atras();
  delay(1000);
  motorB_stop();
  delay(300);

  // --- Pausa antes de repetir ---
  delay(3000);
}