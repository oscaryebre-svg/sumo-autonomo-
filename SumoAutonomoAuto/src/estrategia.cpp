#include "estrategia.h"

// ====================== ayudas de sensores ======================

// ¿Algún QTR está viendo el borde blanco del dohyo?
static bool linea_detectada(const LecturaSensores_t &s) {
  for (uint8_t i = 0; i < 2; i++) {
    bool sobreBlanco = QTR_BORDE_ES_BLANCO ? (s.qtr[i] > UMBRAL_QTR)
                                           : (s.qtr[i] < UMBRAL_QTR);
    if (sobreBlanco) return true;
  }
  return false;
}

// ¿Algún EM-3 está viendo al rival?
static bool rival_detectado(const LecturaSensores_t &s) {
  for (uint8_t i = 0; i < 4; i++) {
    if (s.em3[i]) return true;
  }
  return false;
}

// ====================== estado interno ======================
// (variables estáticas: se conservan entre llamadas)

static uint8_t  estado         = EST_BUSCAR;
static uint8_t  estadoAnterior = EST_BUSCAR;
static uint8_t  faseBorde      = 0;     // 0 = retroceso, 1 = giro
static uint32_t tFase          = 0;     // inicio de la fase actual
static uint32_t tUltimoRival   = 0;     // última vez que se vio al rival
static bool     rivalPorIzq    = true;  // último lado donde se vio al rival
static bool     girarIzq       = true;  // sentido del giro de búsqueda

void estrategia_reiniciar() {
  estado = EST_BUSCAR;
  estadoAnterior = EST_BUSCAR;
  faseBorde = 0;
  tFase = 0;
  tUltimoRival = 0;
  rivalPorIzq = true;
  girarIzq = true;
}

uint8_t estrategia_estado() {
  return estado;
}

Comando_t estrategia_actualizar(const LecturaSensores_t &s, uint32_t t) {
  Comando_t c;
  c.motorIzq = 0;
  c.motorDer = 0;

  // ---------- BORDE: maniobra de escape, NO se interrumpe ----------
  if (estado == EST_BORDE) {
    if (faseBorde == 0) {                    // 1) retroceder
      if (t - tFase < TIEMPO_RETROCESO) {
        c.motorIzq = -VEL_RETROCESO;
        c.motorDer = -VEL_RETROCESO;
        estadoAnterior = estado;
        return c;
      }
      faseBorde = 1;                         // 2) pasar al giro
      tFase = t;
    }
    if (t - tFase < TIEMPO_GIRO_BORDE) {     // 2) girar hacia adentro
      if (girarIzq) {
        c.motorIzq = -VEL_GIRO_BORDE;
        c.motorDer = +VEL_GIRO_BORDE;
      } else {
        c.motorIzq = +VEL_GIRO_BORDE;
        c.motorDer = -VEL_GIRO_BORDE;
      }
      estadoAnterior = estado;
      return c;
    }
    estado = EST_BUSCAR;                     // 3) maniobra terminada
    girarIzq = !girarIzq;                    // la próxima vez gira al otro lado
  }

  // ---------- PRIORIDAD 1: borde del dohyo ----------
  if (linea_detectada(s)) {
    estado = EST_BORDE;
    faseBorde = 0;
    tFase = t;
    c.motorIzq = -VEL_RETROCESO;
    c.motorDer = -VEL_RETROCESO;
    estadoAnterior = estado;
    return c;
  }

  // ---------- PRIORIDAD 2: rival a la vista ----------
  if (rival_detectado(s)) {
    estado = EST_ATAQUE;
    tUltimoRival = t;

    bool izq = s.em3[0] || s.em3[1];   // sensores del lado izquierdo
    bool der = s.em3[2] || s.em3[3];   // sensores del lado derecho

    if (izq && der) {                  // de frente: ¡empujar a tope!
      c.motorIzq = +VEL_ATAQUE;
      c.motorDer = +VEL_ATAQUE;
    } else if (izq) {                  // rival a la izquierda: girar AVANZANDO
      rivalPorIzq = true;              // (arco: se cierra distancia mientras se apunta)
      c.motorIzq = +VEL_ARCO_LENTO;
      c.motorDer = +VEL_ARCO_RAPIDO;
    } else {                           // rival a la derecha: arco hacia allá
      rivalPorIzq = false;
      c.motorIzq = +VEL_ARCO_RAPIDO;
      c.motorDer = +VEL_ARCO_LENTO;
    }
    estadoAnterior = estado;
    return c;
  }

  // ---------- PRIORIDAD 3: buscar ----------
  estado = EST_BUSCAR;

  // Memoria: si el rival se perdió hace poco, seguir girando a ese lado.
  if (t - tUltimoRival < TIEMPO_MEMORIA_LADO) {
    if (rivalPorIzq) {
      c.motorIzq = -VEL_BUSQUEDA;
      c.motorDer = +VEL_BUSQUEDA;
    } else {
      c.motorIzq = +VEL_BUSQUEDA;
      c.motorDer = -VEL_BUSQUEDA;
    }
    estadoAnterior = estado;
    return c;
  }

  // Búsqueda normal: giro CONTINUO en un solo sentido. Así se barre el
  // dohyo completo (360°) y ningún rival queda en un ángulo ciego.
  // (Alternar el sentido sin avanzar solo barre un arco una y otra vez.)
  if (girarIzq) {
    c.motorIzq = -VEL_BUSQUEDA;
    c.motorDer = +VEL_BUSQUEDA;
  } else {
    c.motorIzq = +VEL_BUSQUEDA;
    c.motorDer = -VEL_BUSQUEDA;
  }

  estadoAnterior = estado;
  return c;
}
