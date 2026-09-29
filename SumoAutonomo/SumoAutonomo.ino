// ============================================================
//  SUMO AUTÓNOMO — programa principal
//
//  Flujo: leer sensores -> pedir decisión a la estrategia
//         -> aplicar motores y alas -> (depuración por Serial)
//
//  TODOS los ajustes están en src/config.h
// ============================================================
#include "config.h"
#include "motores.h"
#include "sensores.h"
#include "alas.h"
#include "estrategia.h"

static uint32_t tArranque = 0;   // millis() del arranque del combate

// ¿El módulo de arranque ya dio la señal?
static bool arrancado() {
  bool nivel = (digitalRead(PIN_START) == HIGH);
  // START_ACTIVO_BAJO = 1: el módulo da LOW al arrancar; sin módulo espera.
  // START_ACTIVO_BAJO = 0: corre al encender (sin módulo).
  return START_ACTIVO_BAJO ? !nivel : nivel;
}

void setup() {
  if (DEBUG_SERIAL) {
    Serial.begin(BAUDRATE);
    Serial.println(F("Sumo autonomo: listo"));
  }
  motores_init();
  sensores_init();
  alas_init();
  pinMode(PIN_START, INPUT_PULLUP);
  estrategia_reiniciar();
}

void loop() {
  uint32_t t = millis();

  // Esperar la señal de arranque (módulo MicroStart o similar).
  if (!arrancado()) {
    motores_frenar();
    tArranque = t;
    return;
  }

  LecturaSensores_t s;
  sensores_leer(s);

  Comando_t c = estrategia_actualizar(s, t - tArranque);

  motores_set(c.motorIzq, c.motorDer);
  alas_actualizar(c.patronAlas, t);

  // Depuración: 10 líneas por segundo con todo lo que "ve" el robot.
  if (DEBUG_SERIAL) {
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
      Serial.print(c.motorDer);
      Serial.print(F(" alas="));
      Serial.println(c.patronAlas);
    }
  }
}
