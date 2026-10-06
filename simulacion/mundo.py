"""Mundo 2D simplificado del combate: dohyo, cinemática y sensores virtuales.

Unidades: centímetros y segundos. El comando de motores es el mismo
que usa el código real (-PWM_MAX..+PWM_MAX), así la simulación prueba
la estrategia tal cual corre en el robot.
"""
import math

from constantes import CONST
from estrategia import Lectura

# --- Dohyo de MINISUMO: 77 cm de diámetro, borde blanco de 2,5 cm ---
RADIO_DOHYO = 38.5      # cm (centro a borde exterior)
ANCHO_BORDE = 2.5       # cm
RADIO_NEGRO = RADIO_DOHYO - ANCHO_BORDE   # donde termina el piso negro

# --- Robot ---
DIAMETRO_ROBOT = 10.0   # cm (minisumo: 10 x 10 cm)
V_MAX = 70.0            # cm/s a PWM máximo (medirlo en el test 01 real)
DIST_EJES = 8.0         # cm entre ruedas

# Regla de sumo: pierde el robot cuya punta toca FUERA del dohyo
# (centro más allá de: borde exterior - la mitad del cuerpo).
RADIO_PERDIDA = RADIO_DOHYO - DIAMETRO_ROBOT / 2.0

# --- Sensores ---
ALCANCE_EM3 = 85.0      # cm (alcance real del EM-3)
# Lecturas QTR simuladas, coherentes con QTR_BORDE_ES_BLANCO:
#   flag 1 -> el blanco da mas valor que el negro
#   flag 0 -> el blanco da menos valor que el negro
if CONST["QTR_BORDE_ES_BLANCO"] == 1:
    QTR_BLANCO, QTR_NEGRO = 800, 300
else:
    QTR_BLANCO, QTR_NEGRO = 300, 800


class Robot:
    def __init__(self, x=0.0, y=0.0, angulo=0.0):
        self.x = float(x)
        self.y = float(y)
        self.angulo = float(angulo)   # radianes, 0 = hacia +x
        self.vx = 0.0
        self.vy = 0.0
        self.fuera = False            # se salió del dohyo

    def actualizar(self, izq, der, dt):
        """Cinemática diferencial: izq/der en -PWM_MAX..+PWM_MAX."""
        pmax = float(CONST["PWM_MAX"])
        vl = (izq / pmax) * V_MAX
        vr = (der / pmax) * V_MAX
        v = (vl + vr) / 2.0
        w = (vr - vl) / DIST_EJES

        self.x += v * math.cos(self.angulo) * dt
        self.y += v * math.sin(self.angulo) * dt
        self.angulo += w * dt

        self.vx = v * math.cos(self.angulo)
        self.vy = v * math.sin(self.angulo)

        if math.hypot(self.x, self.y) > RADIO_PERDIDA:
            self.fuera = True


class Oponente(Robot):
    def __init__(self, x, y, conducta):
        super().__init__(x, y)
        self.conducta = conducta

    def mover(self, robot, t, dt):
        izq, der = self.conducta(self, robot, t)
        self.actualizar(izq, der, dt)


def leer_sensores(robot, rival):
    """Devuelve una Lectura tal como la leerían los sensores reales."""
    s = Lectura()

    # Mini QTR: dos puntos en el frontal del robot (±2 cm de lado).
    for i, dy in enumerate((-2.0, 2.0)):
        px = robot.x + math.cos(robot.angulo) * 4.5 - math.sin(robot.angulo) * dy
        py = robot.y + math.sin(robot.angulo) * 4.5 + math.cos(robot.angulo) * dy
        r = math.hypot(px, py)
        s.qtr[i] = QTR_BLANCO if r > RADIO_NEGRO else QTR_NEGRO

    # EM-3: 4 conos en abanico. Convención IGUAL que la estrategia:
    #   em3[0] = +45° (exterior izquierda), em3[1] = +15° (interior izquierda)
    #   em3[2] = -15° (interior derecha),  em3[3] = -45° (exterior derecha)
    # Los conos internos se SOLAPAN al frente (los EM-3 reales no tienen
    # punto ciego): con anchos de 20° cubren de -35° a +35° sin huecos.
    if rival is not None:
        dx = rival.x - robot.x
        dy = rival.y - robot.y
        dist = math.hypot(dx, dy)
        if dist < ALCANCE_EM3:
            rel = math.degrees(math.atan2(dy, dx)) - math.degrees(robot.angulo)
            rel = (rel + 180.0) % 360.0 - 180.0   # normalizar a -180..180
            for i, (centro, ancho) in enumerate(
                    ((45.0, 25.0), (15.0, 20.0), (-15.0, 20.0), (-45.0, 25.0))):
                if abs(rel - centro) <= ancho:
                    s.em3[i] = True
    return s


def resolver_empuje(robot, rival, dt):
    """Modelo simple de contacto: quien avanza empuja al otro."""
    if rival is None:
        return
    dx = rival.x - robot.x
    dy = rival.y - robot.y
    d = math.hypot(dx, dy)
    if d >= DIAMETRO_ROBOT or d < 1e-9:
        return

    ux, uy = dx / d, dy / d

    # Separación: evita que se solapen.
    sep = DIAMETRO_ROBOT - d
    rival.x += ux * sep
    rival.y += uy * sep

    # El robot empuja al rival si avanza contra él.
    v_robot_hacia = robot.vx * ux + robot.vy * uy      # > 0 = avanza hacia el rival
    if v_robot_hacia > 0:
        rival.x += ux * v_robot_hacia * dt * 1.5
        rival.y += uy * v_robot_hacia * dt * 1.5

    # Y el rival empuja al robot si avanza contra él.
    v_rival_hacia = -(rival.vx * ux + rival.vy * uy)   # > 0 = avanza hacia el robot
    if v_rival_hacia > 0:
        robot.x -= ux * v_rival_hacia * dt * 1.5
        robot.y -= uy * v_rival_hacia * dt * 1.5
