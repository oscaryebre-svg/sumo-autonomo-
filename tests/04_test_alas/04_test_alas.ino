// ============================================================
//  TEST 04 — ALAS (servos MOT-110)
//  Barre ambos servos de 0 a 180 grados y de vuelta, imprimiendo
//  el ángulo actual. Sirve para:
//   1. Verificar que las dos alas se mueven.
//   2. Anotar los ángulos MECÁNICOS límite de cada ala (sin forzar)
//      para ajustar ANGULO_ALA_RECOGIDA / ANGULO_ALA_EXTENDIDA.
//   3. Ver si el servo derecho va "espejado" (ALA_DER_INVERTIDA).
//
//  Si un servo se mueve al revés que el otro, reporta cuál.
// ============================================================
#include <Servo.h>

#define PIN_ALA_IZQ 9
#define PIN_ALA_DER 10

Servo alaIzq, alaDer;

void setup() {
  alaIzq.attach(PIN_ALA_IZQ);
  alaDer.attach(PIN_ALA_DER);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS: barrido 0..180..0"));
}

void loop() {
  for (int a = 0; a <= 180; a += 5) {
    alaIzq.write(a);
    alaDer.write(a);
    Serial.println(a);
    delay(100);
  }
  for (int a = 180; a >= 0; a -= 5) {
    alaIzq.write(a);
    alaDer.write(a);
    Serial.println(a);
    delay(100);
  }
}
