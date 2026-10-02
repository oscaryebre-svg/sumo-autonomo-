#include "sensores.h"
#include "config.h"

void sensores_init() {
  pinMode(PIN_EM3_ALA_IZQ, INPUT);
  pinMode(PIN_EM3_CENTRAL_IZQ, INPUT);
  pinMode(PIN_EM3_CENTRAL_DER, INPUT);
  pinMode(PIN_EM3_ALA_DER, INPUT);
  // Los QTR son analógicos: no necesitan pinMode.
}

void sensores_leer(LecturaSensores_t &s) {
  // Orden: 0 = ala izquierda, 1 = central izquierdo,
  //        2 = central derecho, 3 = ala derecha.
  s.em3[0] = (digitalRead(PIN_EM3_ALA_IZQ) == HIGH);
  s.em3[1] = (digitalRead(PIN_EM3_CENTRAL_IZQ) == HIGH);
  s.em3[2] = (digitalRead(PIN_EM3_CENTRAL_DER) == HIGH);
  s.em3[3] = (digitalRead(PIN_EM3_ALA_DER) == HIGH);
  s.qtr[0] = analogRead(PIN_QTR_IZQ);
  s.qtr[1] = analogRead(PIN_QTR_DER);
}
