// ============================================================
//  tipos.h — estructuras de datos compartidas por todos los módulos
// ============================================================
#pragma once
#include <stdint.h>

// Lo que leen los sensores
struct LecturaSensores_t {
  bool em3[4];   // EM-3: true = rival detectado
  int  qtr[2];   // Mini QTR: 0..1023 (ALTO sobre la línea blanca)
};

// Lo que la estrategia le ordena al robot
struct Comando_t {
  int16_t motorIzq;    // -PWM_MAX .. +PWM_MAX (negativo = reversa)
  int16_t motorDer;
};
