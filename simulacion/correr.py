"""Corre la simulación del sumo autónomo.

Uso:
  python correr.py listar              # muestra los escenarios disponibles
  python correr.py <escenario>         # corre un escenario (CSV + gráfica)
  python correr.py torneo --n 30       # torneo con partidas aleatorias
  python correr.py todo                # corre todos los escenarios
"""
import argparse
import csv
import math
import os
import random
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from constantes import CONST
from estrategia import Estrategia, EST_BORDE
from mundo import (
    RADIO_DOHYO, RADIO_NEGRO, RADIO_PERDIDA, Robot, Oponente,
    leer_sensores, resolver_empuje,
)
from escenarios import ESCENARIOS, CONDUCTAS

DT = 0.01          # paso de simulación (s)
T_MAX = 90.0       # duración máxima de un round (s)

RESULTADOS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "resultados")

ENCABEZADO = ["t", "estado", "qtr_izq", "qtr_der",
              "em3_1", "em3_2", "em3_3", "em3_4",
              "motor_izq", "motor_der", "alas",
              "robot_x", "robot_y", "rival_x", "rival_y"]


def simular(esc, nombre, graficar=True):
    """Corre un escenario y devuelve (resultado, duracion, bordes)."""
    estrat = Estrategia()
    robot = Robot(*esc["robot"])
    rival = None
    if esc["rival"]:
        pos, conducta = esc["rival"]
        rival = Oponente(*pos, conducta)

    filas = []
    t = 0.0
    resultado = "empate"
    bordes = 0
    t_ultimo_borde = -10.0

    while t < T_MAX:
        tms = int(t * 1000.0)

        s = leer_sensores(robot, rival)
        c = estrat.actualizar(s, tms)
        robot.actualizar(c.motorIzq, c.motorDer, DT)
        if rival and tms >= CONST["TIEMPO_ESPERA_INICIAL"]:
            rival.mover(robot, t, DT)      # el rival también espera el arranque
        resolver_empuje(robot, rival, DT)

        if estrat.estado == EST_BORDE and t - t_ultimo_borde > 1.0:
            bordes += 1
            t_ultimo_borde = t

        filas.append([t, estrat.estado, s.qtr[0], s.qtr[1],
                      s.em3[0], s.em3[1], s.em3[2], s.em3[3],
                      c.motorIzq, c.motorDer, c.patronAlas,
                      robot.x, robot.y,
                      rival.x if rival else -999.0, rival.y if rival else -999.0])

        if rival and (math.hypot(rival.x, rival.y) > RADIO_PERDIDA or rival.fuera):
            resultado = "victoria"
            break
        if robot.fuera:
            resultado = "derrota"
            break
        t += DT

    os.makedirs(RESULTADOS, exist_ok=True)
    ruta_csv = os.path.join(RESULTADOS, nombre + ".csv")
    with open(ruta_csv, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(ENCABEZADO)
        w.writerows(filas)

    print(f"  -> {esc['descripcion']}")
    print(f"     resultado={resultado}  duracion={t:.1f}s  bordes={bordes}  "
          f"csv={ruta_csv}")
    if graficar:
        grafica(filas, nombre, esc["descripcion"], resultado, t)
    return resultado, t, bordes


def grafica(filas, nombre, descripcion, resultado, duracion):
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except Exception:
        print("     (matplotlib no instalado: sin gráfica, solo CSV)")
        return

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.5))

    # Dohyo
    ax1.set_aspect("equal")
    ax1.add_patch(plt.Circle((0, 0), RADIO_DOHYO,
                             facecolor="white", edgecolor="black", linewidth=2))
    ax1.add_patch(plt.Circle((0, 0), RADIO_NEGRO, facecolor="#222222"))

    xs = [f[11] for f in filas]
    ys = [f[12] for f in filas]
    ax1.plot(xs, ys, color="red", lw=1.5, label="robot")
    xr = [f[13] for f in filas if f[13] != -999.0]
    yr = [f[14] for f in filas if f[13] != -999.0]
    if xr:
        ax1.plot(xr, yr, color="blue", lw=1.5, label="rival")
    ax1.plot(xs[0], ys[0], "o", color="red", ms=4)
    ax1.legend(loc="upper right")
    ax1.set_xlim(-50, 50)
    ax1.set_ylim(-50, 50)
    ax1.set_title(f"{nombre}: {descripcion}\n{resultado} en {duracion:.1f}s")

    # Línea de tiempo de estados
    ts = [f[0] for f in filas]
    es = [f[1] for f in filas]
    ax2.plot(ts, es, drawstyle="steps-post")
    ax2.set_yticks([0, 1, 2, 3])
    ax2.set_yticklabels(["ESPERA", "BUSCAR", "ATAQUE", "BORDE"])
    ax2.set_xlabel("tiempo (s)")
    ax2.set_ylabel("estado")

    fig.tight_layout()
    ruta = os.path.join(RESULTADOS, nombre + ".png")
    fig.savefig(ruta, dpi=110)
    plt.close(fig)
    print(f"     grafica={ruta}")


def torneo(n, graficar=False):
    """Partidas aleatorias contra rivales al azar."""
    victorias = derrotas = empates = 0
    duraciones = []
    for i in range(n):
        random.seed(i)
        robot_pos = (random.uniform(-15, 15), random.uniform(-15, 15),
                     random.uniform(0, 2 * math.pi))
        if random.random() < 0.85:
            rival_pos = (random.uniform(-25, 25), random.uniform(-25, 25))
            rival = (rival_pos, random.choice(CONDUCTAS))
        else:
            rival = None
        esc = {"descripcion": f"partida {i + 1}", "robot": robot_pos, "rival": rival}
        r, dur, _ = simular(esc, f"torneo_p{i + 1:02d}", graficar=graficar)
        duraciones.append(dur)
        if r == "victoria":
            victorias += 1
        elif r == "derrota":
            derrotas += 1
        else:
            empates += 1

    media = sum(duraciones) / max(len(duraciones), 1)
    print(f"\n===== TORNEO: {n} partidas =====")
    print(f"  victorias: {victorias}   derrotas: {derrotas}   empates: {empates}")
    print(f"  tasa de victoria: {100.0 * victorias / n:.0f}%   "
          f"duracion media: {media:.1f}s")

    ruta = os.path.join(RESULTADOS, "torneo_resumen.csv")
    os.makedirs(RESULTADOS, exist_ok=True)
    with open(ruta, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["partida", "resultado", "duracion_s"])
        for i, (r, d) in enumerate(zip(
                ["victoria"] * victorias + ["derrota"] * derrotas + ["empate"] * empates,
                duraciones)):
            w.writerow([i + 1, r, f"{d:.1f}"])
    print(f"  resumen={ruta}")


def main():
    p = argparse.ArgumentParser(description="Simulación del sumo autónomo")
    p.add_argument("comando", nargs="?",
                   help="listar | <nombre de escenario> | torneo | todo")
    p.add_argument("--n", type=int, default=30, help="partidas del torneo")
    p.add_argument("--sin-graficas", action="store_true",
                   help="no generar gráficas (solo CSV)")
    args = p.parse_args()

    if not args.comando or args.comando == "listar":
        print("Escenarios disponibles:")
        for nombre, esc in ESCENARIOS.items():
            print(f"  {nombre:<14} {esc['descripcion']}")
        print("  torneo        partidas aleatorias (--n para cantidad)")
        print("  todo          corre todos los escenarios")
        return

    if args.comando == "torneo":
        torneo(args.n, graficar=False)
        return

    if args.comando == "todo":
        for nombre, esc in ESCENARIOS.items():
            print(f"\n[ESCENARIO {nombre}]")
            simular(esc, nombre, graficar=not args.sin_graficas)
        return

    if args.comando in ESCENARIOS:
        print(f"[ESCENARIO {args.comando}]")
        simular(ESCENARIOS[args.comando], args.comando,
                graficar=not args.sin_graficas)
        return

    p.error(f"escenario desconocido: {args.comando}")


if __name__ == "__main__":
    main()
