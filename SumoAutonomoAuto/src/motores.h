// ============================================================
//  motores.h — control de los 2 motores DC (tracción)
// ============================================================
#pragma once
#include <Arduino.h>
#include "config.h"

// Configura los pines de los motores (llamar en setup)
void motores_init();

// Fija la velocidad de cada motor: -PWM_MAX .. +PWM_MAX
// (negativo = reversa, 0 = frenado)
void motores_set(int16_t izq, int16_t der);

// Frena ambos motores
void motores_frenar();
