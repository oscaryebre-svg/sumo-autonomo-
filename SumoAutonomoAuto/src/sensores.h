// ============================================================
//  sensores.h — lectura de los 4 EM-3 (oponente) y 2 Mini QTR (piso)
// ============================================================
#pragma once
#include <Arduino.h>
#include "tipos.h"

// Configura los pines digitales (llamar en setup)
void sensores_init();

// Llena la estructura con lecturas frescas
void sensores_leer(LecturaSensores_t &s);
