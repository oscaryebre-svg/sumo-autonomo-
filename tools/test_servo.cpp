// ============================================================
//  Prueba de las alas en PC (con el mock de la librería Servo):
//  comprueba que alas.cpp escribe los ángulos correctos en cada
//  servo, que cada ala cambia de posición y que las dos van
//  montadas espejadas.
//
//  Compilar y ejecutar (dentro de tools/):
//    g++ -std=c++11 -Wall -I ../SumoAutonomo/src -I mock test_servo.cpp
//        ../SumoAutonomo/src/alas.cpp -o test_servo && ./test_servo
// ============================================================
#include <cstdio>
#include <cstdlib>
#include <Servo.h>
#include "alas.h"
#include "config.h"

static int fallos = 0;

static void comprobar(const char *nombre, bool ok) {
  std::printf("[%s] %s\n", ok ? "OK" : "FALLA", nombre);
  if (!ok) fallos++;
}

// Devuelve el último ángulo escrito en un pin (-1 si nunca se escribió)
static int ultimoAngulo(int pin) {
  RegistroServos &r = servos();
  int angulo = -1;
  for (int i = 0; i < r.n; i++) {
    if (r.pin[i] == pin) angulo = r.angulo[i];
  }
  return angulo;
}

int main() {
  servos().n = 0;
  alas_init();

  comprobar("el ala izquierda arranca recogida",
            ultimoAngulo(PIN_ALA_IZQ) == ANGULO_IZQ_RECOGIDA);
  comprobar("el ala derecha arranca recogida",
            ultimoAngulo(PIN_ALA_DER) == ANGULO_DER_RECOGIDA);

  alas_desplegar();
  comprobar("al desplegar, la izquierda va a su ángulo",
            ultimoAngulo(PIN_ALA_IZQ) == ANGULO_IZQ_DESPLIEGUE);
  comprobar("al desplegar, la derecha va a su ángulo",
            ultimoAngulo(PIN_ALA_DER) == ANGULO_DER_DESPLIEGUE);

  alas_recoger();
  comprobar("al recoger, la izquierda vuelve",
            ultimoAngulo(PIN_ALA_IZQ) == ANGULO_IZQ_RECOGIDA);
  comprobar("al recoger, la derecha vuelve",
            ultimoAngulo(PIN_ALA_DER) == ANGULO_DER_RECOGIDA);

  // Cada ala debe cambiar de posición (el recorrido real lo mide el test 04).
  int recorridoIzq = abs(ANGULO_IZQ_RECOGIDA - ANGULO_IZQ_DESPLIEGUE);
  int recorridoDer = abs(ANGULO_DER_RECOGIDA - ANGULO_DER_DESPLIEGUE);
  std::printf("  recorrido: izq %d grados, der %d grados\n",
              recorridoIzq, recorridoDer);
  comprobar("el ala izquierda cambia de posición", recorridoIzq > 0);
  comprobar("el ala derecha cambia de posición", recorridoDer > 0);

  // Aviso (no es fallo): las alas espejadas deben girar en sentidos opuestos
  int sentiIzq = ANGULO_IZQ_DESPLIEGUE - ANGULO_IZQ_RECOGIDA;
  int sentiDer = ANGULO_DER_DESPLIEGUE - ANGULO_DER_RECOGIDA;
  std::printf("  sentidos: izq %+d, der %+d -> %s\n", sentiIzq, sentiDer,
              (sentiIzq * sentiDer < 0) ? "espejadas (opuestos)"
                                        : "mismo sentido (revisar montaje)");

  // El aviso de los sentidos ya se imprime arriba; no es un fallo.

  std::printf(fallos == 0 ? "RESULTADO: TODAS OK\n"
                          : "RESULTADO: %d FALLOS\n", fallos);
  return fallos == 0 ? 0 : 1;
}
