#include "alas.h"

// ============================================================
//  Servo por software (sin librería Servo y sin temporizadores).
//
//  Un servo espera un pulso cada 20 ms (50 Hz). El ancho del pulso
//  marca la posición: la recogida y la desplegada se configuran en
//  config.h en microsegundos (PULSO_ALA_RECOGIDA / PULSO_ALA_DESPLIEGUE).
//
//  Las alas solo tienen dos posiciones y la máquina de estados no
//  bloquea el loop, así que los motores y los sensores nunca esperan.
// ============================================================

static uint16_t anchoObjetivo = PULSO_ALA_RECOGIDA;  // µs
static uint32_t tTrama        = 0;   // micros() del inicio de la trama
static uint8_t  paso          = 0;   // 0 espera, 1 pulso izq, 2 pulso der

// Ancho del pulso del ala derecha (espejo dentro del recorrido)
static uint16_t anchoDerecha() {
  if (ALA_DER_INVERTIDA) {
    return (uint16_t)(PULSO_ALA_RECOGIDA + PULSO_ALA_DESPLIEGUE - anchoObjetivo);
  }
  return anchoObjetivo;
}

void alas_init() {
  pinMode(PIN_ALA_IZQ, OUTPUT);
  pinMode(PIN_ALA_DER, OUTPUT);
  digitalWrite(PIN_ALA_IZQ, LOW);
  digitalWrite(PIN_ALA_DER, LOW);
  anchoObjetivo = PULSO_ALA_RECOGIDA;
  tTrama = 0;
  paso = 0;
}

void alas_recoger() {
  anchoObjetivo = PULSO_ALA_RECOGIDA;
}

void alas_desplegar() {
  anchoObjetivo = PULSO_ALA_DESPLIEGUE;
}

void alas_actualizar() {
  uint32_t ahora = micros();

  switch (paso) {
    case 0:                                    // esperar el inicio de trama
      if (ahora - tTrama >= PERIODO_TRAMA_US) {
        tTrama = ahora;
        digitalWrite(PIN_ALA_IZQ, HIGH);
        paso = 1;
      }
      break;

    case 1:                                    // terminar el pulso izquierdo
      if (ahora - tTrama >= anchoObjetivo) {
        digitalWrite(PIN_ALA_IZQ, LOW);
        digitalWrite(PIN_ALA_DER, HIGH);
        paso = 2;
      }
      break;

    case 2:                                    // terminar el pulso derecho
      if (ahora - tTrama >= (uint32_t)anchoObjetivo + anchoDerecha()) {
        digitalWrite(PIN_ALA_DER, LOW);
        paso = 0;
      }
      break;
  }
}
