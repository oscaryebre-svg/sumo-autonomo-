// ============================================================
//  TEST 01 — MOTORES
//  Secuencia repetida: 1) adelante  2) reversa  3) giro izquierda
//  4) giro derecha  5) freno.
//  El Serial avisa qué paso toca en cada momento.
//
//  Reporta al desarrollador:
//   - ¿Avanza derecho? (si tuerce, un motor gira al revés)
//   - ¿Los giros son hacia el lado correcto?
//  Corrección: en config.h cambia DIR_ADELANTE_IZQ / DIR_ADELANTE_DER.
// ============================================================
#define PIN_IZQ_PWM 3
#define PIN_IZQ_DIR 12
#define PIN_DER_PWM 11
#define PIN_DER_DIR 13
#define DIR_ADELANTE_IZQ 1
#define DIR_ADELANTE_DER 1
#define PWM 60

void motor(uint8_t pinPwm, uint8_t pinDir, int vel, uint8_t dirAdelante) {
  digitalWrite(pinDir, vel >= 0 ? dirAdelante : !dirAdelante);
  analogWrite(pinPwm, abs(vel));
}

void frenar() {
  analogWrite(PIN_IZQ_PWM, 0);
  analogWrite(PIN_DER_PWM, 0);
}

void setup() {
  pinMode(PIN_IZQ_PWM, OUTPUT);
  pinMode(PIN_IZQ_DIR, OUTPUT);
  pinMode(PIN_DER_PWM, OUTPUT);
  pinMode(PIN_DER_DIR, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 01 MOTORES (repite la secuencia)"));
}

void loop() {
  Serial.println(F("1) ADELANTE"));
  motor(PIN_IZQ_PWM, PIN_IZQ_DIR, +PWM, DIR_ADELANTE_IZQ);
  motor(PIN_DER_PWM, PIN_DER_DIR, +PWM, DIR_ADELANTE_DER);
  delay(1200);

  Serial.println(F("2) REVERSA"));
  motor(PIN_IZQ_PWM, PIN_IZQ_DIR, -PWM, DIR_ADELANTE_IZQ);
  motor(PIN_DER_PWM, PIN_DER_DIR, -PWM, DIR_ADELANTE_DER);
  delay(1200);

  Serial.println(F("3) GIRO IZQUIERDA"));
  motor(PIN_IZQ_PWM, PIN_IZQ_DIR, -PWM, DIR_ADELANTE_IZQ);
  motor(PIN_DER_PWM, PIN_DER_DIR, +PWM, DIR_ADELANTE_DER);
  delay(1200);

  Serial.println(F("4) GIRO DERECHA"));
  motor(PIN_IZQ_PWM, PIN_IZQ_DIR, +PWM, DIR_ADELANTE_IZQ);
  motor(PIN_DER_PWM, PIN_DER_DIR, -PWM, DIR_ADELANTE_DER);
  delay(1200);

  Serial.println(F("5) FRENO"));
  frenar();
  delay(2000);
}
