// ============================================================
//  alas.h — control de los 2 servos MOT-110 (alas/señuelo)
// ============================================================
#pragma once
#include <Arduino.h>
#include "config.h"

// Configura los servos (llamar en setup)
void alas_init();

// Aplica el patrón indicado a las alas.
// tiempoMs = millis() actual, lo usa el patrón ALA_ONDEO.
void alas_actualizar(uint8_t patron, uint32_t tiempoMs);
