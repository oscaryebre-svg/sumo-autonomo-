// ============================================================
//  TEST 01 — MOTORES (diagnóstico)
//
//  Prueba CADA motor por separado y en cada sentido, para poder
//  distinguir un fallo de HARDWARE (motor, cableado, driver) de un
//  fallo de PROGRAMA (la estrategia).
//
//  Usa los MISMOS pines y la MISMA lógica que motores.cpp: si aquí
//  los motores van bien, el cableado y los motores están bien y el
//  problema estaría en el programa.
//
//  Pines:  IZQ  PWM=D3  DIR=D12      DER  PWM=D11  DIR=D13
//
//  Cómo interpretarlo:
//   - Un motor que NO se mueve en un sentido  -> hardware (cable/driver).
//   - Un motor que gira al REVÉS               -> cambia su DIR_ADELANTE_*.
//   - Los dos bien aquí pero el robot torcido  -> programa (estrategia).
// ============================================================
#define PIN_IZQ_PWM 3
#define PIN_IZQ_DIR 12
#define PIN_DER_PWM 11
#define PIN_DER_DIR 13

// Sentido de avance de cada motor (igual que config.h):
// si un motor gira al revés, cambia su valor de 1 a 0 (o al contrario).
#define DIR_ADELANTE_IZQ 1
#define DIR_ADELANTE_DER 1

#define PWM  60       // velocidad de las pruebas (debe ser <= PWM_MAX de config.h)
#define T_MS 3000     // duración de cada paso (ms)

// Igual que motores.cpp: el signo va al pin DIR, la magnitud al PWM.
void unMotor(uint8_t pinPwm, uint8_t pinDir, int vel, uint8_t dirAdelante) {
  digitalWrite(pinDir, vel >= 0 ? dirAdelante : !dirAdelante);
  analogWrite(pinPwm, abs(vel));
}

void frenar() {
  analogWrite(PIN_IZQ_PWM, 0);
  analogWrite(PIN_DER_PWM, 0);
}

// Imprime lo que se aplica a cada pin (para comparar con el montaje).
void informar(const char *texto, int velIzq, int velDer) {
  Serial.print(F("-> "));
  Serial.println(texto);
  Serial.print(F("   IZQ PWM="));
  Serial.print(abs(velIzq));
  Serial.print(F(" DIR="));
  Serial.print(velIzq >= 0 ? DIR_ADELANTE_IZQ : !DIR_ADELANTE_IZQ);
  Serial.print(F("   DER PWM="));
  Serial.print(abs(velDer));
  Serial.print(F(" DIR="));
  Serial.println(velDer >= 0 ? DIR_ADELANTE_DER : !DIR_ADELANTE_DER);
}

void paso(const char *texto, int velIzq, int velDer) {
  informar(texto, velIzq, velDer);
  unMotor(PIN_IZQ_PWM, PIN_IZQ_DIR, velIzq, DIR_ADELANTE_IZQ);
  unMotor(PIN_DER_PWM, PIN_DER_DIR, velDer, DIR_ADELANTE_DER);
  delay(T_MS);
  frenar();
  delay(1000);
}

void setup() {
  pinMode(PIN_IZQ_PWM, OUTPUT);
  pinMode(PIN_IZQ_DIR, OUTPUT);
  pinMode(PIN_DER_PWM, OUTPUT);
  pinMode(PIN_DER_DIR, OUTPUT);
  frenar();
  Serial.begin(115200);
  Serial.println(F("TEST 01 MOTORES: cada motor por separado"));
  Serial.println(F("Observa QUE rueda se mueve y HACIA DONDE en cada paso."));
}

void loop() {
  // 1) Motor IZQUIERDO solo
  paso("IZQUIERDO solo ADELANTE", +PWM,    0);
  paso("IZQUIERDO solo ATRAS",    -PWM,    0);

  // 2) Motor DERECHO solo
  paso("DERECHO solo ADELANTE",      0, +PWM);
  paso("DERECHO solo ATRAS",         0, -PWM);

  // 3) Los dos juntos
  paso("LOS DOS ADELANTE",        +PWM, +PWM);
  paso("LOS DOS ATRAS",           -PWM, -PWM);

  // 4) Giros (en el sitio)
  paso("GIRO IZQUIERDA (izq atras, der adelante)", -PWM, +PWM);
  paso("GIRO DERECHA (izq adelante, der atras)",   +PWM, -PWM);

  // 5) Rampa de potencia (detecta si el motor necesita más PWM para arrancar)
  Serial.println(F("-> RAMPA DE POTENCIA (los dos adelante)"));
  for (int v = 0; v <= PWM; v += 10) {
    unMotor(PIN_IZQ_PWM, PIN_IZQ_DIR, +v, DIR_ADELANTE_IZQ);
    unMotor(PIN_DER_PWM, PIN_DER_DIR, +v, DIR_ADELANTE_DER);
    Serial.print(F("   PWM="));
    Serial.println(v);
    delay(700);
  }
  frenar();
  delay(3000);
  Serial.println(F("--- fin de la vuelta; se repite ---"));
}
