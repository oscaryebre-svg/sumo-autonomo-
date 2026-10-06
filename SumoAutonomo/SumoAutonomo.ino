// ============================================================
//  SUMO AUTÓNOMO — programa principal
//
//  Secuencia del round:
//    1. Espera la señal de arranque (módulo o START_ACTIVO_BAJO 0).
//    2. Despliega las alas y pelea hasta TIEMPO_COMBATE_MS.
//    3. Al terminar: frena y recoge las alas.
//
//  La variante SumoAutonomoAuto reutiliza este archivo con la macro
//  ARRANQUE_SIN_SENAL = 1 para arrancar al encender, sin señal.
//
//  TODOS los ajustes están en src/config.h
// ============================================================
#include "src/config.h"
#include "src/motores.h"
#include "src/sensores.h"
#include "src/alas.h"
#include "src/estrategia.h"

// Arranque: por defecto espera la señal del módulo de inicio (pin D8).
// La variante SumoAutonomoAuto define esto a 1 para arrancar al encender.
#ifndef ARRANQUE_SIN_SENAL
#define ARRANQUE_SIN_SENAL 0
#endif

static bool     combate         = false;  // ¿hay un round en curso?
static bool     roundTerminado  = false;  // evita reiniciar sin soltar la señal
static bool     alasDesplegadas = false;
static uint32_t tArranque       = 0;      // millis() del inicio del round

// ¿El módulo de arranque ya dio la señal?
static bool arrancado() {
#if ARRANQUE_SIN_SENAL
  return true;   // arranca en cuanto se enciende, sin esperar señal
#else
  bool nivel = (digitalRead(PIN_START) == HIGH);
  // START_ACTIVO_BAJO = 1: el módulo da LOW al arrancar; sin módulo espera.
  // START_ACTIVO_BAJO = 0: corre al encender (sin módulo).
  return START_ACTIVO_BAJO ? !nivel : nivel;
#endif
}

// En el Leonardo (USB nativo) Serial.print BLOQUEA si el Monitor Serial no
// está abierto, y eso congelaría los pulsos de los servos. Por eso solo se
// imprime cuando hay alguien escuchando.
static bool serialListo() {
#if defined(USBCON)
  return (bool)Serial;
#else
  return true;
#endif
}

void setup() {
  if (DEBUG_SERIAL) {
    Serial.begin(BAUDRATE);
    Serial.println(F("Sumo autonomo: listo"));
  }
  motores_init();
  sensores_init();
  alas_init();                        // alas recogidas (posición de medida)
  pinMode(PIN_START, INPUT_PULLUP);   // el pin del módulo queda definido en las dos versiones
  estrategia_reiniciar();
}

void loop() {
  uint32_t t = millis();

  bool senal = arrancado();
  if (!senal) {
    roundTerminado = false;   // al soltar la señal queda listo para otro round
  }

  // ---------- Fuera de combate: frenado y alas recogidas ----------
  if (!combate) {
    motores_frenar();
    alas_recoger();
    alasDesplegadas = false;
    if (senal && !roundTerminado) {
      combate = true;         // empieza el round
      tArranque = t;
    }
    return;
  }

  // ---------- El round termina por señal o por tiempo ----------
  if (!senal || (t - tArranque) >= TIEMPO_COMBATE_MS) {
    combate = false;
    roundTerminado = true;
    return;                   // la siguiente vuelta frena y recoge
  }

  // ---------- Combate en curso: desplegar alas una sola vez ----------
  if (!alasDesplegadas) {
    alas_desplegar();
    alasDesplegadas = true;
  }

  LecturaSensores_t s;
  sensores_leer(s);

  Comando_t c = estrategia_actualizar(s, t - tArranque);

  motores_set(c.motorIzq, c.motorDer);

  // Depuración: 10 líneas por segundo con todo lo que "ve" el robot.
  if (DEBUG_SERIAL && serialListo()) {
    static uint32_t tDbg = 0;
    if (t - tDbg >= 100) {
      tDbg = t;
      Serial.print((t - tArranque) / 1000.0, 1);
      Serial.print(F("s est="));
      Serial.print((int)estrategia_estado());
      Serial.print(F(" qtr="));
      Serial.print(s.qtr[0]);
      Serial.print(F(","));
      Serial.print(s.qtr[1]);
      Serial.print(F(" em3="));
      for (uint8_t i = 0; i < 4; i++) Serial.print(s.em3[i] ? '1' : '0');
      Serial.print(F(" mot="));
      Serial.print(c.motorIzq);
      Serial.print(F(","));
      Serial.println(c.motorDer);
    }
  }
}
