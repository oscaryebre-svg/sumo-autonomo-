// ============================================================
//  TEST 04 — ALAS (servos MOT-110)
//  Las alas tienen 90° de recorrido. Este test las barre de 0° a 90°
//  y de vuelta, imprimiendo el ángulo.
//
//  Sirve para comprobar:
//   1. Que las dos alas se mueven en todo el recorrido sin forzar.
//   2. Si alguna se atasca o choca con el chasis (anotar el ángulo).
//   3. Si el ala derecha va "espejada" (al revés que la izquierda).
// ============================================================
#include <Servo.h>

#define PIN_ALA_IZQ 9
#define PIN_ALA_DER 10
#define RECORRIDO   90      // grados reales de las alas

Servo alaIzq, alaDer;

void setup() {
  alaIzq.attach(PIN_ALA_IZQ);
  alaDer.attach(PIN_ALA_DER);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS: barrido 0..90..0 (recorrido real 90 grados)"));
}

void loop() {
  for (int a = 0; a <= RECORRIDO; a += 5) {
    alaIzq.write(a);
    alaDer.write(a);
    Serial.println(a);
    delay(150);
  }
  for (int a = RECORRIDO; a >= 0; a -= 5) {
    alaIzq.write(a);
    alaDer.write(a);
    Serial.println(a);
    delay(150);
  }
}
