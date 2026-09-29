// ============================================================
//  Prueba de compilación del .ino completo en PC (sin Arduino):
//  compila SumoAutonomo.ino + todos los módulos con el mock y
//  ejecuta setup() + loop() unas cuantas veces. Si compila y corre,
//  el código es válido para la placa (salvo diferencias del core).
//
//  Compilar y ejecutar (desde la raíz del proyecto), en una sola línea:
//    g++ -std=c++11 -Wall -I SumoAutonomo/src -I tools/mock
//        -x c++ SumoAutonomo/SumoAutonomo.ino tools/test_ino.cpp
//        SumoAutonomo/src/estrategia.cpp SumoAutonomo/src/motores.cpp
//        SumoAutonomo/src/sensores.cpp SumoAutonomo/src/alas.cpp
//        -o tools/test_ino && ./tools/test_ino
// ============================================================
#include <cstdio>

// Funciones del sketch (definidas en el .ino compilado junto a esto)
extern void setup();
extern void loop();

int main() {
  std::printf("Compilando y ejecutando el .ino con mock...\n");
  setup();
  for (int i = 0; i < 5; i++) loop();
  std::printf("OK: el .ino completo compila y corre sin placa.\n");
  return 0;
}
