// Mock de la librería Servo para compilar y probar en PC.
// Registra los attach() y write() para poder verificarlos desde los tests.
#pragma once
#include <stdint.h>

struct RegistroServos {
  static const int MAX = 16;
  int n;
  int pin[MAX];
  int angulo[MAX];
};
inline RegistroServos &servos() { static RegistroServos r = {0, {0}, {0}}; return r; }

class Servo {
  int miPin;

public:
  Servo() : miPin(-1) {}

  void attach(int pin) { miPin = pin; }

  void write(int angulo) {
    RegistroServos &r = servos();
    if (r.n < RegistroServos::MAX) {
      r.pin[r.n] = miPin;
      r.angulo[r.n] = angulo;
      r.n++;
    }
  }

  int pin() const { return miPin; }
};
