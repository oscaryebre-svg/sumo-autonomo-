#include "sensores.h"
#include "config.h"

void sensores_init() {
  pinMode(PIN_EM3_1, INPUT);
  pinMode(PIN_EM3_2, INPUT);
  pinMode(PIN_EM3_3, INPUT);
  pinMode(PIN_EM3_4, INPUT);
  // Los QTR son analógicos: no necesitan pinMode.
}

void sensores_leer(LecturaSensores_t &s) {
  s.em3[0] = (digitalRead(PIN_EM3_1) == HIGH);
  s.em3[1] = (digitalRead(PIN_EM3_2) == HIGH);
  s.em3[2] = (digitalRead(PIN_EM3_3) == HIGH);
  s.em3[3] = (digitalRead(PIN_EM3_4) == HIGH);
  s.qtr[0] = analogRead(PIN_QTR_IZQ);
  s.qtr[1] = analogRead(PIN_QTR_DER);
}
