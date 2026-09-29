// ============================================================
//  tipos.h — estructuras de datos compartidas por todos los módulos
// ============================================================
#pragma once
#include <stdint.h>

// Patrones de movimiento de las alas (señuelo para el rival)
enum PatronAlas : uint8_t {
  ALA_QUIETAS    = 0,  // dejar los servos donde están
  ALA_RECOGIDAS  = 1,  // pegadas al cuerpo (perfil bajo)
  ALA_EXTENDIDAS = 2,  // abiertas al máximo (silueta grande)
  ALA_ONDEO      = 3   // oscilación continua
};

// Lo que leen los sensores
struct LecturaSensores_t {
  bool em3[4];   // EM-3: true = rival detectado
  int  qtr[2];   // Mini QTR: 0..1023 (ALTO sobre la línea blanca)
};

// Lo que la estrategia le ordena al robot
struct Comando_t {
  int16_t motorIzq;    // -PWM_MAX .. +PWM_MAX (negativo = reversa)
  int16_t motorDer;
  uint8_t patronAlas;  // PatronAlas
};
