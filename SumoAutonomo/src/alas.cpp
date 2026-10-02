#include "alas.h"

// ============================================================
//  Servo por software (sin librería Servo y sin temporizadores).
//
//  Un servo espera un pulso cada 20 ms (50 Hz): 1 ms ≈ 0° y 2 ms ≈ 180°.
//  Como las alas solo recorren 90°, se usa 1 ms (recogidas) a 2 ms
//  (desplegadas). La máquina de estados no bloquea el loop.
// ============================================================

static int      anguloObjetivo = ANGULO_ALA_RECOGIDA;  // 0..90
static uint32_t tTrama         = 0;   // micros() del inicio de la trama
static uint8_t  paso           = 0;   // 0 espera, 1 pulso izq, 2 pulso der

// Convierte 0..90 grados en la duración del pulso (1000..2000 microsegundos)
static uint16_t duracionPulso(int angulo) {
  angulo = constrain(angulo, 0, 90);
  return (uint16_t)(1000 + (angulo * 1000L) / 90);
}

void alas_init() {
  pinMode(PIN_ALA_IZQ, OUTPUT);
  pinMode(PIN_ALA_DER, OUTPUT);
  digitalWrite(PIN_ALA_IZQ, LOW);
  digitalWrite(PIN_ALA_DER, LOW);
  anguloObjetivo = ANGULO_ALA_RECOGIDA;
  tTrama = 0;
  paso = 0;
}

void alas_recoger() {
  anguloObjetivo = ANGULO_ALA_RECOGIDA;
}

void alas_desplegar() {
  anguloObjetivo = ANGULO_ALA_DESPLIEGUE;
}

void alas_actualizar() {
  uint32_t ahora = micros();
  uint16_t durIzq = duracionPulso(anguloObjetivo);
  uint16_t durDer = duracionPulso(ALA_DER_INVERTIDA ? (90 - anguloObjetivo)
                                                    : anguloObjetivo);

  switch (paso) {
    case 0:                                    // esperar el inicio de trama
      if (ahora - tTrama >= PERIODO_TRAMA_US) {
        tTrama = ahora;
        digitalWrite(PIN_ALA_IZQ, HIGH);
        paso = 1;
      }
      break;

    case 1:                                    // terminar el pulso izquierdo
      if (ahora - tTrama >= durIzq) {
        digitalWrite(PIN_ALA_IZQ, LOW);
        digitalWrite(PIN_ALA_DER, HIGH);
        paso = 2;
      }
      break;

    case 2:                                    // terminar el pulso derecho
      if (ahora - tTrama >= (uint32_t)durIzq + durDer) {
        digitalWrite(PIN_ALA_DER, LOW);
        paso = 0;
      }
      break;
  }
}
