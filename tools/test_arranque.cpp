// ============================================================
//  Prueba del ARRANQUE en PC (sin Arduino):
//  simula el pin del módulo de arranque (PIN_START) y comprueba
//  que el robot NO se mueve en reposo y SÍ arranca al recibir la
//  señal, según el START_MODO de config.h.
//
//  Compilar y ejecutar (desde la raíz del proyecto):
//    g++ -std=c++11 -Wall -I SumoAutonomo/src -I tools/mock
//        -x c++ SumoAutonomo/SumoAutonomo.ino tools/test_arranque.cpp
//        SumoAutonomo/src/estrategia.cpp SumoAutonomo/src/motores.cpp
//        SumoAutonomo/src/sensores.cpp SumoAutonomo/src/alas.cpp
//        -o tools/test_arranque && ./tools/test_arranque
// ============================================================
#include <cstdio>
#include "Arduino.h"   // nivelPin(), pwmPin(), analogPin()
#include "config.h"
#include "alas.h"
#include "motores.h"
#include "sensores.h"
#include "estrategia.h"

// Funciones del sketch (definidas en el .ino compilado junto a esto)
extern void setup();
extern void loop();

static int fallos = 0;

static void comprobar(const char *nombre, bool ok) {
  std::printf("[%s] %s\n", ok ? "OK" : "FALLA", nombre);
  if (!ok) fallos++;
}

// Deja el piso en "negro" (sin línea) para que la estrategia no escape.
static void piso_negro() {
#if QTR_BORDE_ES_BLANCO
  analogPin(PIN_QTR_IZQ) = 200;   // negro = valor bajo si el blanco es alto
  analogPin(PIN_QTR_DER) = 200;
#else
  analogPin(PIN_QTR_IZQ) = 800;   // negro = valor alto si el blanco es bajo
  analogPin(PIN_QTR_DER) = 800;
#endif
}

int main() {
#if START_MODO == 1
  const int REPOSO = HIGH, SENAL = LOW;   // módulo activo BAJO
#else
  const int REPOSO = LOW, SENAL = HIGH;   // módulo activo ALTO (MicroStart)
#endif

  // Estado inicial del mock
  for (int i = 0; i < 32; i++) {
    pwmPin(i) = 0;
    nivelPin(i) = LOW;
    analogPin(i) = 0;
  }
  piso_negro();

  setup();

#if START_MODO == 0
  // Sin módulo: el robot arranca al encender.
  loop();
  comprobar("sin modulo (START_MODO 0): los motores se mueven al encender",
            pwmPin(PIN_MOTOR_IZQ_PWM) != 0 || pwmPin(PIN_MOTOR_DER_PWM) != 0);
  std::printf("  (START_MODO 0: no hay señal que probar)\n");
#else
  // 1) En reposo NO debe arrancar.
  nivelPin(PIN_START) = REPOSO;
  loop();
  comprobar("reposo: motor izquierdo frenado", pwmPin(PIN_MOTOR_IZQ_PWM) == 0);
  comprobar("reposo: motor derecho frenado", pwmPin(PIN_MOTOR_DER_PWM) == 0);

  // 2) Con la señal DEBE arrancar y mover los motores.
  //    El primer ciclo arranca el round (despliega alas) y sale; los motores
  //    reciben la orden en el ciclo siguiente.
  nivelPin(PIN_START) = SENAL;
  loop();
  loop();
  comprobar("senal: motor izquierdo se mueve", pwmPin(PIN_MOTOR_IZQ_PWM) != 0);
  comprobar("senal: motor derecho se mueve", pwmPin(PIN_MOTOR_DER_PWM) != 0);

  // 3) Al retirar la señal, el round termina y vuelve a frenar.
  nivelPin(PIN_START) = REPOSO;
  loop();   // detecta el fin del round
  loop();   // ya fuera de combate: frena
  comprobar("fin de senal: vuelve a frenar",
            pwmPin(PIN_MOTOR_IZQ_PWM) == 0 && pwmPin(PIN_MOTOR_DER_PWM) == 0);
#endif

  std::printf(fallos == 0 ? "RESULTADO: TODAS OK\n"
                          : "RESULTADO: %d FALLOS\n", fallos);
  return fallos == 0 ? 0 : 1;
}
