// ============================================================
//  TEST 04 — ALAS (servos en D4 izquierda y D2 derecha)
//
//  Mueve las alas DESPACIO y de forma continua entre los dos pulsos
//  de config.h (recogida y despliegue), imprimiendo el pulso en
//  microsegundos y el ángulo equivalente.
//
//  Los pines D2/D4 no son de temporizador, así que los pulsos se
//  generan a mano: no hace falta instalar ninguna librería.
//
//  Sirve para:
//   1. Comprobar que el movimiento es suave, sin tirones ni pausas.
//   2. Encontrar el pulso donde el ala se atasca o zumba (límite
//      mecánico) y ajustar PULSO_ALA_* en config.h.
//   3. Ver si el ala derecha va "espejada" (al revés que la izquierda).
// ============================================================
#define PIN_ALA_IZQ 4
#define PIN_ALA_DER 2

#define PULSO_MIN  1000   // µs (posición recogida)
#define PULSO_MAX  1600   // µs (un poco más allá del despliegue, por si hay margen)
#define PASO_US      10   // µs que avanza en cada paso (movimiento suave)

void pulso(uint8_t pin, int ancho) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(ancho);
  digitalWrite(pin, LOW);
}

void mover(int ancho) {
  pulso(PIN_ALA_IZQ, ancho);          // una trama de 20 ms para cada servo
  pulso(PIN_ALA_DER, ancho);
  delay(20);
  int grados = (ancho - 1000) * 180L / 1000;   // 1000..2000 µs ≈ 0..180°
  Serial.print(ancho);
  Serial.print(F(" us (~"));
  Serial.print(grados);
  Serial.println(F(" grados)"));
}

void setup() {
  pinMode(PIN_ALA_IZQ, OUTPUT);
  pinMode(PIN_ALA_DER, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS (D4 izq, D2 der): barrido suave 1000..1600 us"));
}

void loop() {
  for (int a = PULSO_MIN; a <= PULSO_MAX; a += PASO_US) mover(a);
  for (int a = PULSO_MAX; a >= PULSO_MIN; a -= PASO_US) mover(a);
}
