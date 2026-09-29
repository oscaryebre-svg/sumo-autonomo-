"""Escenarios de simulación: posiciones iniciales y conducta del rival."""
import math

from constantes import CONST


def _pm():
    return CONST["PWM_MAX"]


# ---------- conductas del rival (scripteadas) ----------

def conducta_quieto(o, robot, t):
    return (0, 0)


def conducta_carga(o, robot, t):
    """Apunta al robot y lo embiste cuando lo tiene de frente."""
    dx, dy = robot.x - o.x, robot.y - o.y
    dist = math.hypot(dx, dy)
    if dist < 70.0 and dist > 1e-6:
        rel = math.degrees(math.atan2(dy, dx)) - math.degrees(o.angulo)
        rel = (rel + 180.0) % 360.0 - 180.0
        if abs(rel) < 20.0:
            return (_pm(), _pm())              # embestir
        # rel < 0 = el robot está a la DERECHA -> girar a la derecha (CW)
        return (35, -35) if rel < 0 else (-35, 35)
    return (28, -28)                           # girar buscando


def conducta_orbita(o, robot, t):
    return (30, 60)


def conducta_huye(o, robot, t):
    dx, dy = robot.x - o.x, robot.y - o.y
    if math.hypot(dx, dy) < 45.0:
        return (-_pm(), -_pm())
    return (0, 0)


CONDUCTAS = [conducta_quieto, conducta_carga, conducta_orbita, conducta_huye]


# ---------- escenarios ----------

ESCENARIOS = {
    "rival_frente": {
        "descripcion": "Rival quieto de frente: debe atacarlo y empujarlo",
        "robot": (0.0, -15.0, math.pi / 2),
        "rival": ((0.0, 10.0), conducta_quieto),
    },
    "rival_lateral": {
        "descripcion": "Rival a la izquierda: debe girar fino y atacar",
        "robot": (0.0, -10.0, math.pi / 2),
        "rival": ((-15.0, 8.0), conducta_quieto),
    },
    "sin_rival": {
        "descripcion": "Dohyo vacío: debe buscar sin salirse del borde",
        "robot": (0.0, 0.0, 0.0),
        "rival": None,
    },
    "borde": {
        "descripcion": "Empieza con el morro sobre el borde: debe escapar",
        "robot": (0.0, -32.5, -math.pi / 2),
        "rival": None,
    },
    "embestida": {
        "descripcion": "El rival carga contra el robot: aguantar y responder",
        "robot": (0.0, 10.0, -math.pi / 2),
        "rival": ((0.0, -15.0), conducta_carga),
    },
    "orbita": {
        "descripcion": "Rival que orbita: localizarlo y perseguirlo",
        "robot": (0.0, 0.0, 0.0),
        "rival": ((15.0, 0.0), conducta_orbita),
    },
}
