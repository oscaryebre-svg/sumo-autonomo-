// ============================================================
//  estrategia.h — máquina de estados del combate.
//
//  Es lógica PURA: no toca pines, no usa delay() ni Serial.
//  Por eso se puede:
//   1) compilar en PC con un mock (tools/test_estrategia.cpp) y
//   2) portar 1:1 a Python (simulacion/estrategia.py).
//
//  IMPORTANTE: si cambias la lógica aquí, cambia también
//  simulacion/estrategia.py para que sigan siendo idénticos.
// ============================================================
#pragma once
#include <stdint.h>
#include "config.h"

// Estados del robot (también para la depuración por Serial)
enum EstadoRobot : uint8_t {
  EST_BUSCAR = 0,   // girar buscando al rival
  EST_ATAQUE = 1,   // rival a la vista: apuntar y empujar
  EST_BORDE  = 2    // maniobra de escape del borde
};

// Vuelve al estado inicial (buscar). Llamar en setup.
void estrategia_reiniciar();

// Estado actual (para imprimirlo por Serial)
uint8_t estrategia_estado();

// Decide el próximo comando a partir de las lecturas y el tiempo.
// tiempoMs = millis() desde el arranque del combate.
Comando_t estrategia_actualizar(const LecturaSensores_t &s, uint32_t tiempoMs);
