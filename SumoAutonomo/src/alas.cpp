#include "alas.h"
#include <Servo.h>

static Servo alaIzq, alaDer;

// Escribe un ángulo en un servo, respetando el modo "espejo".
static void escribir_ala(Servo &servo, bool espejo, int angulo) {
  angulo = constrain(angulo, 0, 180);
  servo.write(espejo ? (180 - angulo) : angulo);
}

void alas_init() {
  alaIzq.attach(PIN_ALA_IZQ);
  alaDer.attach(PIN_ALA_DER);
  escribir_ala(alaIzq, false, ANGULO_ALA_RECOGIDA);
  escribir_ala(alaDer, ALA_DER_INVERTIDA != 0, ANGULO_ALA_RECOGIDA);
}

void alas_actualizar(uint8_t patron, uint32_t t) {
  int angulo;

  switch (patron) {
    case ALA_RECOGIDAS:
      angulo = ANGULO_ALA_RECOGIDA;
      break;

    case ALA_EXTENDIDAS:
      angulo = ANGULO_ALA_EXTENDIDA;
      break;

    case ALA_ONDEO: {
      // Oscilación triangular alrededor del centro del recorrido:
      // sube la primera mitad del periodo y baja la segunda.
      int centro = (ANGULO_ALA_RECOGIDA + ANGULO_ALA_EXTENDIDA) / 2;
      uint32_t fase = t % (uint32_t)PERIODO_ONDEO_MS;
      uint32_t mitad = (uint32_t)PERIODO_ONDEO_MS / 2;
      if (fase < mitad) {
        angulo = centro - AMPLITUD_ONDEO
               + (int)((fase * 2 * (uint32_t)AMPLITUD_ONDEO) / (uint32_t)PERIODO_ONDEO_MS);
      } else {
        angulo = centro + AMPLITUD_ONDEO
               - (int)((((fase - mitad) * 2 * (uint32_t)AMPLITUD_ONDEO)) / (uint32_t)PERIODO_ONDEO_MS);
      }
      break;
    }

    case ALA_QUIETAS:
    default:
      return;  // dejar las alas donde están
  }

  escribir_ala(alaIzq, false, angulo);
  escribir_ala(alaDer, ALA_DER_INVERTIDA != 0, angulo);
}
