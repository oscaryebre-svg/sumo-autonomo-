"""Puerto 1:1 de SumoAutonomo/src/estrategia.cpp

MISMA lógica, MISMOS nombres, MISMAS constantes (vía constantes.py).
IMPORTANTE: si cambias la lógica en estrategia.cpp, cámbiala aquí también
(idealmente antes, para probarla en simulación y luego copiarla al C++).
"""
from constantes import CONST

# Estados (igual que el enum EstadoRobot de estrategia.h)
EST_ESPERA, EST_BUSCAR, EST_ATAQUE, EST_BORDE = 0, 1, 2, 3

# Patrones de alas (igual que el enum PatronAlas de tipos.h)
ALA_QUIETAS, ALA_RECOGIDAS, ALA_EXTENDIDAS, ALA_ONDEO = 0, 1, 2, 3

# Mismos valores que PATRON_ALAS_* en config.h (sincronizar a mano)
PATRON_ALAS_ATAQUE = ALA_EXTENDIDAS
PATRON_ALAS_BUSQUEDA = ALA_ONDEO
PATRON_ALAS_BORDE = ALA_QUIETAS


class Lectura:
    __slots__ = ("em3", "qtr")

    def __init__(self, em3=None, qtr=None):
        self.em3 = [False] * 4 if em3 is None else list(em3)
        self.qtr = [0, 0] if qtr is None else list(qtr)


class Comando:
    __slots__ = ("motorIzq", "motorDer", "patronAlas")

    def __init__(self, motorIzq=0, motorDer=0, patronAlas=ALA_QUIETAS):
        self.motorIzq = motorIzq
        self.motorDer = motorDer
        self.patronAlas = patronAlas

    def __repr__(self):
        return (f"Comando(izq={self.motorIzq}, der={self.motorDer}, "
                f"alas={self.patronAlas})")


# ---- ayudas de sensores (igual que en estrategia.cpp) ----

def linea_detectada(s):
    blanco_alto = CONST["QTR_BORDE_ES_BLANCO"] == 1
    umbral = CONST["UMBRAL_QTR"]
    for v in s.qtr:
        if (v > umbral) if blanco_alto else (v < umbral):
            return True
    return False


def rival_detectado(s):
    return any(s.em3)


class Estrategia:
    """Máquina de estados: ESPERA -> BUSCAR/ATAQUE/BORDE."""

    def __init__(self):
        self.reiniciar()

    def reiniciar(self):
        self.estado = EST_ESPERA
        self.estadoAnterior = EST_ESPERA
        self.faseBorde = 0
        self.tFase = 0
        self.tUltimoRival = 0
        self.rivalPorIzq = True
        self.girarIzq = True

    def actualizar(self, s, t):
        c = Comando()

        # ---------- ESPERA ----------
        if self.estado == EST_ESPERA:
            c.patronAlas = ALA_RECOGIDAS
            if t < CONST["TIEMPO_ESPERA_INICIAL"]:
                self.estadoAnterior = self.estado
                return c
            self.estado = EST_BUSCAR

        # ---------- BORDE (no se interrumpe) ----------
        if self.estado == EST_BORDE:
            if self.faseBorde == 0:                       # retroceder
                if t - self.tFase < CONST["TIEMPO_RETROCESO"]:
                    c.motorIzq = -CONST["VEL_RETROCESO"]
                    c.motorDer = -CONST["VEL_RETROCESO"]
                    self.estadoAnterior = self.estado
                    return c
                self.faseBorde = 1
                self.tFase = t
            if t - self.tFase < CONST["TIEMPO_GIRO_BORDE"]:  # girar
                if self.girarIzq:
                    c.motorIzq = -CONST["VEL_GIRO_BORDE"]
                    c.motorDer = +CONST["VEL_GIRO_BORDE"]
                else:
                    c.motorIzq = +CONST["VEL_GIRO_BORDE"]
                    c.motorDer = -CONST["VEL_GIRO_BORDE"]
                self.estadoAnterior = self.estado
                return c
            self.estado = EST_BUSCAR                     # terminada
            self.girarIzq = not self.girarIzq

        # ---------- PRIORIDAD 1: borde ----------
        if linea_detectada(s):
            self.estado = EST_BORDE
            self.faseBorde = 0
            self.tFase = t
            c.motorIzq = -CONST["VEL_RETROCESO"]
            c.motorDer = -CONST["VEL_RETROCESO"]
            c.patronAlas = PATRON_ALAS_BORDE
            self.estadoAnterior = self.estado
            return c

        # ---------- PRIORIDAD 2: rival ----------
        if rival_detectado(s):
            self.estado = EST_ATAQUE
            self.tUltimoRival = t
            c.patronAlas = PATRON_ALAS_ATAQUE

            izq = s.em3[0] or s.em3[1]
            der = s.em3[2] or s.em3[3]

            if izq and der:                     # de frente: empujar
                c.motorIzq = +CONST["VEL_ATAQUE"]
                c.motorDer = +CONST["VEL_ATAQUE"]
            elif izq:                           # rival a la izquierda: arco
                self.rivalPorIzq = True
                c.motorIzq = +CONST["VEL_ARCO_LENTO"]
                c.motorDer = +CONST["VEL_ARCO_RAPIDO"]
            else:                               # rival a la derecha: arco
                self.rivalPorIzq = False
                c.motorIzq = +CONST["VEL_ARCO_RAPIDO"]
                c.motorDer = +CONST["VEL_ARCO_LENTO"]
            self.estadoAnterior = self.estado
            return c

        # ---------- PRIORIDAD 3: buscar ----------
        self.estado = EST_BUSCAR
        c.patronAlas = PATRON_ALAS_BUSQUEDA

        if t - self.tUltimoRival < CONST["TIEMPO_MEMORIA_LADO"]:
            if self.rivalPorIzq:
                c.motorIzq = -CONST["VEL_BUSQUEDA"]
                c.motorDer = +CONST["VEL_BUSQUEDA"]
            else:
                c.motorIzq = +CONST["VEL_BUSQUEDA"]
                c.motorDer = -CONST["VEL_BUSQUEDA"]
            self.estadoAnterior = self.estado
            return c

        # Búsqueda normal: giro CONTINUO en un solo sentido (barre 360°).
        if self.girarIzq:
            c.motorIzq = -CONST["VEL_BUSQUEDA"]
            c.motorDer = +CONST["VEL_BUSQUEDA"]
        else:
            c.motorIzq = +CONST["VEL_BUSQUEDA"]
            c.motorDer = -CONST["VEL_BUSQUEDA"]

        self.estadoAnterior = self.estado
        return c
