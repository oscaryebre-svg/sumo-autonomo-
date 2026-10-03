"""Carga las constantes numéricas de config.h para que la simulación use
EXACTAMENTE los mismos valores que el código del robot.

Cada vez que cambies config.h, la simulación se actualiza sola.
Solo se sincronizan los valores numéricos (velocidades, tiempos, umbrales
y ángulos de las alas).
"""
import os
import re

RUTA_CONFIG = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "..", "SumoAutonomo", "src", "config.h",
)

INTERES = [
    "PWM_MAX",
    "VEL_BUSQUEDA", "VEL_ATAQUE", "VEL_ARCO_LENTO", "VEL_ARCO_RAPIDO",
    "VEL_GIRO_BORDE", "VEL_RETROCESO",
    "TIEMPO_RETROCESO", "TIEMPO_GIRO_BORDE", "TIEMPO_MEMORIA_LADO",
    "UMBRAL_QTR", "QTR_BORDE_ES_BLANCO",
    "PULSO_ALA_RECOGIDA", "PULSO_ALA_DESPLIEGUE",
]

_PATRON = re.compile(r"^#define\s+(\w+)\s+(\S+)")


def _leer_crudos():
    crudos = {}
    with open(RUTA_CONFIG, encoding="utf-8") as f:
        for linea in f:
            m = _PATRON.match(linea.strip())
            if m:
                crudos[m.group(1)] = m.group(2)
    return crudos


def _resolver(nombre, crudos):
    """Resuelve cadenas de macros, p. ej. VEL_ATAQUE -> PWM_MAX -> 70."""
    valor = crudos[nombre]
    while not valor.isdigit():
        if valor not in crudos:
            raise KeyError(
                f"No se pudo resolver la macro '{valor}' usada por '{nombre}'"
            )
        valor = crudos[valor]
    return int(valor)


CRUDOS = _leer_crudos()
CONST = {n: _resolver(n, CRUDOS) for n in INTERES if n in CRUDOS}
