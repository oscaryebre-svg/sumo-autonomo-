// Mock de la librería Servo para compilar en PC (no hace nada).
#pragma once
#include <stdint.h>

class Servo {
public:
  void attach(int) {}
  void write(int) {}
};
