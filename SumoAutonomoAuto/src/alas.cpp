#include "alas.h"
#include <Servo.h>

// Cada ala tiene sus propios ángulos porque van montadas espejadas.
static Servo alaIzq;
static Servo alaDer;

void alas_init() {
  alaIzq.attach(PIN_ALA_IZQ);
  alaDer.attach(PIN_ALA_DER);
  alas_recoger();
}

void alas_recoger() {
  alaIzq.write(ANGULO_IZQ_RECOGIDA);
  alaDer.write(ANGULO_DER_RECOGIDA);
}

void alas_desplegar() {
  alaIzq.write(ANGULO_IZQ_DESPLIEGUE);
  alaDer.write(ANGULO_DER_DESPLIEGUE);
}
