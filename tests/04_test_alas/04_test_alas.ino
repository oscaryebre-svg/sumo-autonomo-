// ============================================================
//  TEST 04 — ALAS (servos MOT-110 en D2 y D4)
//  Las alas tienen 90° de recorrido. Este test las barre de 0° a 90°
//  y de vuelta, imprimiendo el ángulo.
//
//  D2 y D4 no son pines de temporizador, así que este test genera los
//  pulsos a mano (igual que el programa principal): no hace falta
//  instalar ninguna librería.
//
//  Sirve para comprobar:
//   1. Que las dos alas se mueven en todo el recorrido sin forzar.
//   2. Si alguna se atasca o choca con el chasis (anotar el ángulo).
//   3. Si el ala derecha va "espejada" (al revés que la izquierda).
// ============================================================
#define PIN_ALA_IZQ 4
#define PIN_ALA_DER 2
#define RECORRIDO   90      // grados reales de las alas

// Envía un pulso de servo: 0° -> 1 ms, 90° -> 2 ms
void pulso(uint8_t pin, int grados) {
  int us = 1000 + (grados * 1000L) / 90;
  digitalWrite(pin, HIGH);
  delayMicroseconds(us);
  digitalWrite(pin, LOW);
}

// Repite el pulso varias veces para que el servo llegue y se mantenga
void mover(int grados) {
  Serial.println(grados);
  for (int i = 0; i < 10; i++) {
    pulso(PIN_ALA_IZQ, grados);
    pulso(PIN_ALA_DER, grados);
    delay(20);
  }
}

void setup() {
  pinMode(PIN_ALA_IZQ, OUTPUT);
  pinMode(PIN_ALA_DER, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS en D4(izq) y D2(der): barrido 0..90..0"));
}

void loop() {
  for (int a = 0; a <= RECORRIDO; a += 5) mover(a);
  for (int a = RECORRIDO; a >= 0; a -= 5) mover(a);
}
