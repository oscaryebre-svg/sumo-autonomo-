#include "alas.h"
#include <Servo.h>

static Servo alaIzq, alaDer;

// Escribe el mismo ángulo (0..90) en las dos alas, respetando el espejo.
static void escribir_ambas(int angulo) {
  angulo = constrain(angulo, 0, 90);   // recorrido real de las alas: 90°
  alaIzq.write(angulo);
  alaDer.write(ALA_DER_INVERTIDA ? (90 - angulo) : angulo);
}

void alas_init() {
  alaIzq.attach(PIN_ALA_IZQ);
  alaDer.attach(PIN_ALA_DER);
  alas_recoger();
}

void alas_recoger() {
  escribir_ambas(ANGULO_ALA_RECOGIDA);
}

void alas_desplegar() {
  escribir_ambas(ANGULO_ALA_DESPLIEGUE);
}
