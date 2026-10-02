// ============================================================
//  Prueba de la lógica en PC (sin Arduino):
//  compila estrategia.cpp (y el resto de módulos) con un mock
//  de Arduino y verifica la máquina de estados con sensores
//  simulados. Es la MISMA lógica que corre en el robot.
//
//  Compilar y ejecutar (dentro de tools/), en una sola línea:
//    g++ -std=c++11 -Wall -I ../SumoAutonomo/src -I . test_estrategia.cpp
//        ../SumoAutonomo/src/estrategia.cpp ../SumoAutonomo/src/motores.cpp
//        ../SumoAutonomo/src/sensores.cpp ../SumoAutonomo/src/alas.cpp
//        -o test_estrategia && ./test_estrategia
// ============================================================
#include <cstdio>
#include "estrategia.h"
#include "motores.h"
#include "sensores.h"
#include "alas.h"

static int fallos = 0;

static void comprobar(const char *nombre, bool ok) {
  std::printf("[%s] %s\n", ok ? "OK" : "FALLA", nombre);
  if (!ok) fallos++;
}

int main() {
  LecturaSensores_t s;
  for (int i = 0; i < 4; i++) s.em3[i] = false;
  s.qtr[0] = 300;   // piso negro (bajo el umbral)
  s.qtr[1] = 300;
  estrategia_reiniciar();

  // 1. Sin rival: debe girar (buscar). No hay cuenta regresiva previa.
  Comando_t c = estrategia_actualizar(s, 0);
  comprobar("buscar: gira en el sitio", c.motorIzq != 0 || c.motorDer != 0);
  comprobar("estado inicial: buscar", estrategia_estado() == EST_BUSCAR);

  // 2. Rival de frente: empuje a tope con ambos motores.
  s.em3[1] = true;
  s.em3[2] = true;
  c = estrategia_actualizar(s, 100);
  comprobar("ataque frontal: empuja",
            c.motorIzq > 0 && c.motorDer > 0 && c.motorIzq == c.motorDer);
  s.em3[1] = false;
  s.em3[2] = false;

  // 3. Rival a la izquierda: arco hacia la izquierda (avanza girando).
  s.em3[0] = true;
  c = estrategia_actualizar(s, 200);
  comprobar("ataque lateral izq: arco izq",
            c.motorIzq >= 0 && c.motorDer > c.motorIzq);
  s.em3[0] = false;

  // 4. Borde: retroceso inmediato.
  s.qtr[0] = 800;
  c = estrategia_actualizar(s, 300);
  comprobar("borde: retrocede", c.motorIzq < 0 && c.motorDer < 0);

  // 5. La maniobra de borde NO se interrumpe por el rival.
  s.qtr[0] = 300;
  s.em3[2] = true;
  c = estrategia_actualizar(s, 400);
  comprobar("borde: no interrumpe por rival", c.motorIzq < 0 && c.motorDer < 0);
  s.em3[2] = false;

  // 6. Tras el borde (180 ms + 420 ms) vuelve a buscar.
  c = estrategia_actualizar(s, 1000);
  comprobar("borde: vuelve a buscar", c.motorIzq != 0 || c.motorDer != 0);

  // 7. Memoria: rival visto a la izquierda, se pierde y sigue girando
  //    hacia la izquierda durante TIEMPO_MEMORIA_LADO.
  s.em3[0] = true;
  c = estrategia_actualizar(s, 2000);
  s.em3[0] = false;
  c = estrategia_actualizar(s, 2100);
  comprobar("memoria: gira hacia el último lado", c.motorIzq < 0 && c.motorDer > 0);

  // 8. Humo: el resto de módulos compilan y corren sin placa.
  motores_init();
  motores_set(10, -10);
  motores_frenar();
  sensores_init();
  sensores_leer(s);
  alas_init();
  alas_desplegar();
  alas_recoger();
  comprobar("humo: modulos sin placa", true);

  std::printf(fallos == 0 ? "RESULTADO: TODAS OK\n"
                          : "RESULTADO: %d FALLOS\n", fallos);
  return fallos == 0 ? 0 : 1;
}
