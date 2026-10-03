# Documentación del Desarrollador — Robot de Sumo Autónomo

Guía para quien programa el robot (sin acceso al hardware).
Lo que ve el tester está en `TUTORIAL_TESTER.md`; este documento NO se
entrega al tester.

## 1. Probar la lógica en PC (sin Arduino, sin librerías)

Compila `estrategia.cpp` (y el resto de módulos) con un mock de Arduino y
verifica la máquina de estados con sensores simulados:

```bash
cd "tools"
g++ -std=c++11 -Wall -I ../SumoAutonomo/src -I . \
  test_estrategia.cpp ../SumoAutonomo/src/estrategia.cpp \
  ../SumoAutonomo/src/motores.cpp ../SumoAutonomo/src/sensores.cpp \
  ../SumoAutonomo/src/alas.cpp -o test_estrategia
./test_estrategia          # debe terminar con "RESULTADO: TODAS OK"
```

También mide los pulsos que se envían a los servos (ancho y periodo de
trama), con un reloj controlable:

```bash
cd "tools"
g++ -std=c++11 -Wall -I ../SumoAutonomo/src -I . test_servo.cpp \
  ../SumoAutonomo/src/alas.cpp -o test_servo
./test_servo               # debe terminar con "RESULTADO: TODAS OK"
```

También se puede compilar y ejecutar el `.ino` completo en PC:

```bash
g++ -std=c++11 -Wall -I SumoAutonomo/src -I tools/mock \
  -x c++ SumoAutonomo/SumoAutonomo.ino tools/test_ino.cpp \
  SumoAutonomo/src/estrategia.cpp SumoAutonomo/src/motores.cpp \
  SumoAutonomo/src/sensores.cpp SumoAutonomo/src/alas.cpp \
  -o tools/test_ino
./tools/test_ino
```

## 2. Simulación 2D (Python)

```bash
cd "simulacion"
source ~/Proyectos/.venv/bin/activate     # venv compartido de ~/Proyectos
python correr.py listar                   # escenarios disponibles
python correr.py rival_frente             # un escenario (CSV + gráfica)
python correr.py torneo --n 30            # torneo aleatorio
python correr.py todo                     # todos los escenarios
python correr.py todo --sin-graficas      # igual pero sin PNG (solo CSV)
```

Genera `simulacion/resultados/*.csv` (traza completa) y `*.png`
(trayectorias sobre el dohyo y línea de tiempo de estados).

## 3. Compilar para la placa real

```bash
sudo pacman -S arduino-cli               # una sola vez
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:leonardo SumoAutonomo/
# si la placa es estilo Nano:
arduino-cli compile --fqbn arduino:avr:nano SumoAutonomo/
```

Con Arduino IDE 2.x: abrir `SumoAutonomo/SumoAutonomo.ino`, elegir placa y
compilar. No hace falta instalar ninguna librería: los motores, los sensores
y los servos se manejan con pines directos (los servos en D4/D2 generan sus
pulsos por software, así que tampoco se usa la librería Servo).

## 4. Flujo de trabajo con el tester

1. El tester sigue `TUTORIAL_TESTER.md` y entrega el formulario relleno.
2. Con esos datos se ajusta SOLO `SumoAutonomo/src/config.h`:
   - Motores al revés → `DIR_ADELANTE_IZQ` / `DIR_ADELANTE_DER`
   - Nombres de los EM-3 → `PIN_EM3_ALA_IZQ`, `PIN_EM3_CENTRAL_IZQ`,
     `PIN_EM3_CENTRAL_DER`, `PIN_EM3_ALA_DER`
   - Umbral QTR → `UMBRAL_QTR = (negro + blanco) / 2`
   - Pulsos de las alas → `PULSO_ALA_RECOGIDA` / `PULSO_ALA_DESPLIEGUE`
   - Alas espejadas → `ALA_DER_INVERTIDA`
   - Duración del round → `TIEMPO_COMBATE_MS`
   - Sin módulo de arranque → `START_ACTIVO_BAJO 0`
   - Buck de 12 V → `PWM_MAX 255`; 6S directa → `PWM_MAX 70`
3. Se recompila, se sube a la rama y el tester repite la prueba.
4. Las líneas del Serial del robot real se pueden comparar con la traza de
   la simulación (`resultados/*.csv`) para detectar divergencias.

## 5. Sincronización simulación ↔ código real

- `simulacion/constantes.py` lee `config.h`: los números siempre coinciden.
- `simulacion/estrategia.py` es un puerto **1:1** de
  `SumoAutonomo/src/estrategia.cpp`. Si cambias la lógica en uno, cámbiala en
  el otro (se recomienda probar primero en simulación y luego copiar).
- `tools/test_estrategia.cpp` compila el C++ real en PC con un mock: verifica
  la lógica exacta que corre en el robot, sin la placa.

## 6. Subir cambios a GitHub

```bash
cd "/home/oskrpc/Proyectos/sumo autonomo"
git add -A && git commit -m "descripción del cambio"
# con GitHub CLI (tras gh auth login):
git push origin programacion-sumo
# o con token (sin gh):
TOKEN=$(grep -oE '"GITHUB_PERSONAL_ACCESS_TOKEN": *"[^"]*"' \
  ~/.config/opencode/opencode.jsonc | sed -E 's/.*"([^"]+)"$/\1/')
git push "https://x-access-token:${TOKEN}@github.com/oscaryebre-svg/sumo-autonomo-.git" \
  programacion-sumo
```

Rama en GitHub: `oscaryebre-svg/sumo-autonomo-` → rama `programacion-sumo`.

## 7. Pendientes de datos reales

- Umbral QTR real y alturas de montaje (test 03).
- `PWM_MAX` según la alimentación (buck 12 V = 255; 6S directa = 70).
- Sentido de giro real de cada motor (test 01).
- Ajustar con el test 04 los pulsos de los topes reales de cada ala
  (`PULSO_ALA_RECOGIDA` / `PULSO_ALA_DESPLIEGUE`) y `ALA_DER_INVERTIDA`.
  El MOT-110 es analógico de 180°: los pulsos válidos van de 600 a 2400 µs
  y el test barre 700–2300 µs para medir los topes.
