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

  // 1. Espera inicial: motores quietos.
  Comando_t c = estrategia_actualizar(s, 0);
  comprobar("espera: motores quietos", c.motorIzq == 0 && c.motorDer == 0);
  comprobar("espera: estado correcto", estrategia_estado() == EST_ESPERA);

  // 2. Sin rival: debe girar (buscar).
  c = estrategia_actualizar(s, 6000);
  comprobar("buscar: gira en el sitio", c.motorIzq != 0 || c.motorDer != 0);

  // 3. Rival de frente: empuje a tope con ambos motores.
  s.em3[1] = true;
  s.em3[2] = true;
  c = estrategia_actualizar(s, 6200);
  comprobar("ataque frontal: empuja",
            c.motorIzq > 0 && c.motorDer > 0 && c.motorIzq == c.motorDer);
  s.em3[1] = false;
  s.em3[2] = false;

  // 4. Rival a la izquierda: arco hacia la izquierda (avanza girando).
  s.em3[0] = true;
  c = estrategia_actualizar(s, 6400);
  comprobar("ataque lateral izq: arco izq",
            c.motorIzq >= 0 && c.motorDer > c.motorIzq);
  s.em3[0] = false;

  // 5. Borde: retroceso inmediato.
  s.qtr[0] = 800;
  c = estrategia_actualizar(s, 6600);
  comprobar("borde: retrocede", c.motorIzq < 0 && c.motorDer < 0);

  // 6. La maniobra de borde NO se interrumpe por el rival.
  s.qtr[0] = 300;
  s.em3[2] = true;
  c = estrategia_actualizar(s, 6700);
  comprobar("borde: no interrumpe por rival", c.motorIzq < 0 && c.motorDer < 0);
  s.em3[2] = false;

  // 7. Tras el borde (180 ms + 420 ms) vuelve a buscar.
  c = estrategia_actualizar(s, 7300);
  comprobar("borde: vuelve a buscar", c.motorIzq != 0 || c.motorDer != 0);

  // 8. Memoria: rival visto a la izquierda, se pierde y sigue girando
  //    hacia la izquierda durante TIEMPO_MEMORIA_LADO.
  s.em3[0] = true;
  c = estrategia_actualizar(s, 8000);
  s.em3[0] = false;
  c = estrategia_actualizar(s, 8100);
  comprobar("memoria: gira hacia el último lado", c.motorIzq < 0 && c.motorDer > 0);

  // 9. Humo: el resto de módulos compilan y corren sin placa.
  motores_init();
  motores_set(10, -10);
  motores_frenar();
  sensores_init();
  sensores_leer(s);
  alas_init();
  alas_actualizar(ALA_ONDEO, 500);
  comprobar("humo: modulos sin placa", true);

  std::printf(fallos == 0 ? "RESULTADO: TODAS OK\n"
                          : "RESULTADO: %d FALLOS\n", fallos);
  return fallos == 0 ? 0 : 1;
}
