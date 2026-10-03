// ============================================================
//  Prueba del servo por software en PC (sin placa):
//  ejecuta alas.cpp con un reloj controlable y MIDE los pulsos
//  generados (ancho y periodo) para el servo MOT-110.
//
//  Compilar y ejecutar (dentro de tools/):
//    g++ -std=c++11 -Wall -I ../SumoAutonomo/src -I . test_servo.cpp
//        ../SumoAutonomo/src/alas.cpp -o test_servo && ./test_servo
// ============================================================
#include <cstdio>
#include "alas.h"
#include "config.h"

static int fallos = 0;

static void comprobar(const char *nombre, bool ok) {
  std::printf("[%s] %s\n", ok ? "OK" : "FALLA", nombre);
  if (!ok) fallos++;
}

struct Pulsos {
  int n;
  unsigned long inicio[32];   // instante en que empieza cada pulso
  unsigned long ancho[32];    // duración de cada pulso
};

// Extrae los pulsos completos (HIGH -> LOW) del pin indicado
static Pulsos extraer(uint8_t pinBuscado) {
  Pulsos p;
  p.n = 0;
  bool alto = false;
  unsigned long tAlto = 0;
  RegistroPines &r = registro();
  for (int i = 0; i < r.n; i++) {
    if (r.pin[i] != pinBuscado) continue;
    if (r.nivel[i] == HIGH && !alto) {
      alto = true;
      tAlto = r.t[i];
    } else if (r.nivel[i] == LOW && alto) {
      alto = false;
      if (p.n < 32) {
        p.inicio[p.n] = tAlto;
        p.ancho[p.n] = r.t[i] - tAlto;
        p.n++;
      }
    }
  }
  return p;
}

// Avanza el reloj en pasos de 20 µs durante "ms" milisegundos
static void ejecutar(unsigned long &t, int ms) {
  for (int i = 0; i < ms * 50; i++) {
    microsActual() = t;
    alas_actualizar();
    t += 20;
  }
}

static bool cerca(long valor, long esperado, long tolerancia) {
  long dif = valor - esperado;
  return dif > -tolerancia && dif < tolerancia;
}

int main() {
  microsActual() = 0;
  registro().n = 0;
  alas_init();

  unsigned long t = 0;
  ejecutar(t, 80);

  Pulsos izq = extraer(PIN_ALA_IZQ);
  Pulsos der = extraer(PIN_ALA_DER);

  comprobar("se generan pulsos en el ala izquierda", izq.n >= 2);
  comprobar("se generan pulsos en el ala derecha", der.n >= 2);

  if (izq.n >= 1) {
    comprobar("recogida: ancho de pulso ~PULSO_ALA_RECOGIDA",
              cerca((long)izq.ancho[0], PULSO_ALA_RECOGIDA, 80));
  }
  if (izq.n >= 2) {
    comprobar("el periodo entre tramas es 20 ms (50 Hz)",
              cerca((long)(izq.inicio[1] - izq.inicio[0]), PERIODO_TRAMA_US, 100));
  }
  if (izq.n >= 1 && der.n >= 1) {
    comprobar("ala derecha con el mismo ancho (sin espejo)",
              cerca((long)der.ancho[0] - (long)izq.ancho[0], 0, 80));
  }

  // Ahora despliega las alas y vuelve a medir
  registro().n = 0;
  alas_desplegar();
  t += 5000;
  ejecutar(t, 80);
  Pulsos izq2 = extraer(PIN_ALA_IZQ);

  comprobar("despliegue: ancho de pulso ~PULSO_ALA_DESPLIEGUE",
            izq2.n >= 1 &&
            cerca((long)izq2.ancho[0], PULSO_ALA_DESPLIEGUE, 80));

  // Y las recoge de nuevo
  registro().n = 0;
  alas_recoger();
  t += 5000;
  ejecutar(t, 80);
  Pulsos izq3 = extraer(PIN_ALA_IZQ);

  comprobar("recoger: vuelve al ancho de recogida",
            izq3.n >= 1 &&
            cerca((long)izq3.ancho[0], PULSO_ALA_RECOGIDA, 80));

  // Todos los pulsos deben estar dentro del rango seguro del servo
  bool rangoOk = true;
  for (int i = 0; i < izq3.n; i++) {
    if (izq3.ancho[i] < PULSO_MINIMO_US || izq3.ancho[i] > PULSO_MAXIMO_US) rangoOk = false;
  }
  comprobar("los pulsos quedan dentro del rango seguro (600-2400 us)", rangoOk);

  std::printf(fallos == 0 ? "RESULTADO: TODAS OK\n"
                          : "RESULTADO: %d FALLOS\n", fallos);
  return fallos == 0 ? 0 : 1;
}
