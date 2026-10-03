// ============================================================
//  TEST 04 — ALAS (servos MOT-110 en D4 izquierda y D2 derecha)
//
//  El MOT-110 es un servo analógico de 180°: necesita pulsos cada
//  20 ms y mantiene la posición solo mientras los recibe.
//
//  Este test barre TODO el rango útil (700 a 2300 µs) despacio y de
//  forma continua, imprimiendo el pulso. El ángulo que muestra es
//  aproximado (convención 1000-2000 µs = 0-180°); lo que manda es el µs.
//
//  Sirve para:
//   1. Comprobar que el movimiento es suave, sin tirones ni pausas.
//   2. Anotar el µs en el que el ala está TOTALMENTE RECOGIDA y el µs
//      en el que está TOTALMENTE DESPLEGADA (sus topes mecánicos).
//      Si zumba en un extremo, ese µs ya pasa del tope: anótalo y sigue.
//   3. Ver si el ala derecha va "espejada" (al revés que la izquierda).
// ============================================================
#define PIN_ALA_IZQ 4
#define PIN_ALA_DER 2

#define PULSO_MIN  700    // µs (por debajo del recorrido normal)
#define PULSO_MAX  2300   // µs (por encima del recorrido normal)
#define PASO_US     20    // µs que avanza en cada paso (movimiento suave)

void pulso(uint8_t pin, int ancho) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(ancho);
  digitalWrite(pin, LOW);
}

void mover(int ancho) {
  pulso(PIN_ALA_IZQ, ancho);          // una trama de 20 ms para cada servo
  pulso(PIN_ALA_DER, ancho);
  delay(20);
  int grados = (ancho - 1000) * 180L / 1000;   // aproximado
  Serial.print(ancho);
  Serial.print(F(" us  (~"));
  Serial.print(grados);
  Serial.println(F(" grados aprox.)"));
}

void setup() {
  pinMode(PIN_ALA_IZQ, OUTPUT);
  pinMode(PIN_ALA_DER, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS (D4 izq, D2 der): barrido suave 700..2300 us"));
  Serial.println(F("Anota el us del ala RECOGIDA y el del ala DESPLEGADA."));
}

void loop() {
  for (int a = PULSO_MIN; a <= PULSO_MAX; a += PASO_US) mover(a);
  for (int a = PULSO_MAX; a >= PULSO_MIN; a -= PASO_US) mover(a);
}
