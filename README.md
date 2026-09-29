# Robot de Sumo Autónomo — Programación

Control de un robot de sumo autónomo para Arduino (placa XMotion/Genesis de
JSumo, estilo Leonardo/Nano) con estrategia de combate, alas de engaño y
simulación 2D para desarrollo sin el robot físico.

## Componentes

| Componente | Rol | Tipo de señal |
|---|---|---|
| 2 motores Core 750 RPM (6 V) | Tracción diferencial | PWM + dirección |
| 4 sensores EM-3 (E-Robots) | Detectar al rival (frente) | Digital (1 = rival) |
| 2 sensores Mini QTR (E-Robots) | Detectar el borde del dohyo | Analógico (0–1023) |
| 2 servos MOT-110 (Steren) | Alas/señuelo | PWM de servo |
| Batería LiPo 6S 22,2 V 850 mAh | Alimentación | ⚠️ ver abajo |

## ⚠️ Alimentación (importante)

- La placa admite **máximo 15 V** (según versión). La batería de **22,2 V NO
  debe conectarse directo** a la entrada de la placa.
- **Recomendado:** regulador reductor (buck) de **12 V / 3–5 A** entre la
  batería y la placa. Entonces `PWM_MAX = 255` en `config.h`.
- Si se conecta directo (solo placas que soporten 24 V): `PWM_MAX = 70`
  (27 % ≈ 6 V efectivos para los motores).
- Usa una **alarma LiPo externa** en el conector de balance (la placa V3 no
  trae divisor de voltaje).

## Estructura del proyecto

```
sumo autonomo/
├── SumoAutonomo/
│   ├── SumoAutonomo.ino      ← programa principal (loop + depuración)
│   └── src/
│       ├── config.h          ← ★ TODOS los ajustes (pines, velocidades…)
│       ├── tipos.h           ← estructuras compartidas
│       ├── motores.h/.cpp    ← control de motores (D3/D12/D11/D13)
│       ├── sensores.h/.cpp   ← EM-3 (A4/A5/D1/D0) y QTR (A1/A2)
│       ├── alas.h/.cpp       ← servos MOT-110 y patrones
│       └── estrategia.h/.cpp ← máquina de estados (lógica pura)
├── simulacion/
│   ├── constantes.py         ← lee config.h (sincronización automática)
│   ├── estrategia.py         ← puerto 1:1 de estrategia.cpp
│   ├── mundo.py              ← dohyo, cinemática y sensores virtuales
│   ├── escenarios.py         ← 6 escenarios de combate
│   └── correr.py             ← ejecuta simulación, CSV y gráficas
├── tests/                    ← sketches de calibración para el tester
├── tools/                    ← mock de Arduino + prueba de lógica en PC
└── README.md
```

## Pinout

| Función | Pin | Notas |
|---|---|---|
| Motor izquierdo PWM | D3 | |
| Motor izquierdo DIR | D12 | |
| Motor derecho PWM | D11 | |
| Motor derecho DIR | D13 | |
| QTR izquierdo (piso) | A1 | analógico |
| QTR derecho (piso) | A2 | analógico |
| EM-3 1 | A4 | orden físico por confirmar (test 02) |
| EM-3 2 | A5 | |
| EM-3 3 | D1 | ⚠️ D0/D1 son el Serial1: no usar Serial1 en el código |
| EM-3 4 | D0 | |
| Ala izquierda (servo) | D9 | confirmar pines PWM libres en la placa |
| Ala derecha (servo) | D10 | |
| Módulo de arranque | D8 | `INPUT_PULLUP`, opcional |

## Cómo funciona la estrategia

Máquina de estados con prioridades (en cada ciclo del `loop()`):

1. **BORDE** (máxima prioridad): si un QTR ve la línea blanca → retrocede
   180 ms y gira 420 ms hacia adentro (el lado se alterna).
2. **ATAQUE**: si un EM-3 ve al rival → gira fino hacia ese lado; si lo
   tiene de frente, empuja a `VEL_ATAQUE` con las alas extendidas.
3. **BUSCAR**: si no hay nadie → gira en el sitio en un solo sentido (barre
   los 360° del dohyo) y recuerda el último lado donde vio al rival durante
   800 ms.
4. **ESPERA**: 5 s de cuenta regresiva tras la señal de arranque.

Los tiempos y velocidades se ajustan en `config.h` sin tocar la lógica.

## Desarrollo sin el robot (lo que haces tú)

### 1. Probar la lógica en PC (sin Arduino, sin librerías)

```bash
cd "tools"
g++ -std=c++11 -Wall \
    -I ../SumoAutonomo/src -I . \
    test_estrategia.cpp \
    ../SumoAutonomo/src/estrategia.cpp \
    ../SumoAutonomo/src/motores.cpp \
    ../SumoAutonomo/src/sensores.cpp \
    ../SumoAutonomo/src/alas.cpp \
    -o test_estrategia
./test_estrategia
```

Debe terminar con `RESULTADO: TODAS OK`.

### 2. Simulación 2D (Python)

```bash
cd "simulacion"
source ~/Proyectos/.venv/bin/activate     # venv compartido de ~/Proyectos
python correr.py listar
python correr.py rival_frente
python correr.py torneo --n 30
python correr.py todo
```

Genera `resultados/*.csv` (traza completa) y `resultados/*.png` (trayectorias
sobre el dohyo y línea de tiempo de estados).

### 3. Compilar el código real (requiere entorno Arduino)

```bash
sudo pacman -S arduino-cli            # una sola vez
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:leonardo SumoAutonomo/
# si la placa es estilo Nano:
arduino-cli compile --fqbn arduino:avr:nano SumoAutonomo/
```

Con Arduino IDE 2.x (descargar de arduino.cc, AppImage en Linux): abrir
`SumoAutonomo/SumoAutonomo.ino`, seleccionar placa y compilar. **No hace
falta** instalar la librería XMotion: el código maneja los pines directo.

## Calibración con el robot (lo que hace el tester)

Baudios del Serial Monitor: **115200**.

| Test | Qué se calibra | Qué reporta el tester |
|---|---|---|
| `tests/01_test_motores` | Sentido de giro de los motores | Si avanza derecho y si los giros son correctos |
| `tests/02_test_em3` | Orden de los 4 EM-3 | Qué sensor físico es cada columna (de izq. a der.) |
| `tests/03_test_qtr` | Umbral del QTR | Valores sobre negro y sobre blanco |
| `tests/04_test_alas` | Límites de las alas | Ángulos mecánicos máximos y si el servo derecho va espejado |

Con esos datos se actualiza `config.h` y se recompila. Después, con
`DEBUG_SERIAL = 1` el programa principal imprime 10 veces por segundo:

```
12.3s est=2 qtr=340,355 em3=0100 mot=70,70 alas=2
```

Es decir: tiempo, estado (0 espera, 1 buscar, 2 ataque, 3 borde),
lecturas QTR, los 4 EM-3, comandos a motores y patrón de alas.
El tester copia estas líneas y el desarrollador ajusta `config.h`.

### Subir el código (Arduino IDE, para el tester)

1. Instalar Arduino IDE 2.x desde <https://www.arduino.cc/en/software>.
2. `File → Open` → `SumoAutonomo.ino` (no renombrar la carpeta).
3. `Tools → Board` → "Arduino Leonardo" (o "Arduino Nano" según la placa).
4. `Tools → Port` → el puerto que aparezca al conectar por USB.
5. Botón **Upload** (→). Para ver los valores: `Tools → Serial Monitor`,
   baudios 115200.

## Solución de problemas

| Síntoma | Causa probable | Solución |
|---|---|---|
| El robot avanza girando | Un motor va al revés | Cambiar `DIR_ADELANTE_IZQ`/`DIR_ADELANTE_DER` en config.h |
| Gira al lado contrario en el borde | Igual | Ídem |
| No detecta la línea blanca | `UMBRAL_QTR` mal calibrado | Recalibrar con test 03 |
| Los servos tiemblan o los motores zumban raro | Conflicto de timers con Servo en esa placa | Probar servos en D5/D9 (ver `PIN_ALA_*`) |
| No arranca nunca | Módulo de arranque ausente | Poner `START_ACTIVO_BAJO 0` en config.h |
| Se calientan los motores | PWM muy alto con 6S directa | Bajar `PWM_MAX` o usar buck de 12 V |

## Sincronización simulación ↔ código real

- `simulacion/constantes.py` lee `config.h`: los números siempre coinciden.
- `simulacion/estrategia.py` es un puerto **1:1** de
  `SumoAutonomo/src/estrategia.cpp`. Si cambias la lógica en uno, cámbiala en
  el otro (se recomienda probar primero en simulación y luego copiar).
- `tools/test_estrategia.cpp` compila el C++ real en PC con un mock: verifica
  la lógica exacta que corre en el robot, sin la placa.
