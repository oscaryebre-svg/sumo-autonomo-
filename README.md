# Robot de Sumo Autónomo — Programación

Programa de prototipado de un robot de sumo autónomo con 2 alas de servos.

Control de un robot de sumo autónomo para Arduino (placa XMotion/Genesis de
JSumo, estilo Leonardo/Nano) con estrategia de combate, alas que llevan los
sensores de búsqueda del rival y simulación 2D para desarrollo sin el robot
físico.

## Documentos

| Documento | Para quién |
|---|---|
| **`TUTORIAL_TESTER.md`** | **El tester**: instalación, subida del código, calibración y formulario de reporte |
| **`DOCUMENTACION_DESARROLLADOR.md`** | El desarrollador: simulación, pruebas en PC, compilación y sincronización |

## Componentes

| Componente | Rol | Tipo de señal |
|---|---|---|
| 2 motores Core 750 RPM (6 V) | Tracción diferencial | PWM + dirección |
| 4 sensores EM-3 (E-Robots) | Detectar al rival (2 van en las alas) | Digital (1 = rival) |
| 2 sensores Mini QTR (E-Robots) | Detectar el borde del dohyo | Analógico (0–1023) |
| 2 servos MOT-110 (Steren) | Alas retráctiles con los EM-3 de búsqueda | Librería Servo · analógico 3,5–6 V, 40 mA |
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

## Pinout

| Función | Pin | Notas |
|---|---|---|
| Motor izquierdo PWM | D3 | |
| Motor izquierdo DIR | D12 | |
| Motor derecho PWM | D11 | |
| Motor derecho DIR | D13 | |
| QTR izquierdo (piso) | A1 | analógico; detecta el borde blanco (prioridad máxima) |
| QTR derecho (piso) | A2 | analógico; detecta el borde blanco (prioridad máxima) |
| EM-3 ala izquierda | D0 | extremo del ala izquierda · lado IZQUIERDO · ⚠️ D0/D1 = Serial1 (Leonardo) / Serial (Nano) |
| EM-3 central izquierdo | D1 | junto al morro, lado izquierdo |
| EM-3 central derecho | A5 | junto al morro, lado derecho |
| EM-3 ala derecha | A4 | extremo del ala derecha · lado DERECHO |
| Servo ala izquierda | D4 | librería Servo |
| Servo ala derecha | D2 | librería Servo |
| Módulo de arranque | D10 | opcional; polaridad en `START_MODO` (ver test 05) |

## Cómo funciona la estrategia

Máquina de estados con prioridades (en cada ciclo del `loop()`):

1. **BORDE** (máxima prioridad): si un QTR ve la línea blanca → retrocede
   180 ms y gira 420 ms hacia adentro (el lado se alterna).
2. **ATAQUE**: si un EM-3 ve al rival → gira avanzando en arco hacia ese
   lado; si lo tiene de frente, empuja a `VEL_ATAQUE`.
3. **BUSCAR**: si no hay nadie → gira en el sitio en un solo sentido (barre
   los 360° del dohyo) y recuerda el último lado donde vio al rival durante
   800 ms.

Las **alas** llevan montados los **EM-3 de los extremos**, que son los que
**buscan al enemigo**: se recogen al inicio (posición de
medida), se **extienden al recibir la señal de inicio** (en la variante
`SumoAutonomoAuto`, al encender) y se recogen al terminar el round. Se
controlan con la librería **Servo** (incluida en el IDE de Arduino) en los
pines D4 (izquierda) y D2 (derecha). Las dos alas se recogen a 90° y se
extienden 90° en sentidos opuestos porque van montadas espejadas (izquierda
90°→0°, derecha 90°→180°), definidos en `config.h`.

El round dura `TIEMPO_COMBATE_MS` (3 min por defecto): al cumplirse, el
robot frena y recoge las alas.

El programa `SumoAutonomo` espera la señal del módulo de arranque; la
variante `SumoAutonomoAuto` empieza el round en cuanto se enciende.

El **módulo de arranque** se conecta a `D10`. Su polaridad se ajusta con
`START_MODO` en `config.h`: `2` = activo alto (JSumo MicroStart: en reposo
da 0 V y al dar la señal 5 V), `1` = activo bajo (pulsador con pull-up),
`0` = sin módulo (arranca al encender). Se comprueba con el **test 05**.

Los tiempos y velocidades se ajustan en `config.h` sin tocar la lógica.

## Estructura del proyecto

```
sumo autonomo/
├── README.md                          ← este resumen
├── TUTORIAL_TESTER.md                 ← ★ tutorial y formulario del tester
├── DOCUMENTACION_DESARROLLADOR.md     ← guía del desarrollador
├── SumoAutonomo/                      ← programa principal (espera señal)
│   ├── SumoAutonomo.ino
│   └── src/                           ← módulos (config, motores, sensores,
│                                         alas, estrategia)
├── SumoAutonomoAuto/                  ← variante que arranca al encender
│   ├── SumoAutonomoAuto.ino
│   └── src/                           ← copia de SumoAutonomo/src
├── simulacion/                        ← simulación 2D (uso: guía desarrollador)
├── tests/                             ← 5 sketches de calibración (tester)
└── tools/                             ← pruebas de lógica en PC (desarrollador)
```

## Solución de problemas

| Síntoma | Causa probable | Solución |
|---|---|---|
| El robot avanza girando | Un motor va al revés | Cambiar `DIR_ADELANTE_IZQ`/`DIR_ADELANTE_DER` en config.h |
| Gira al lado contrario en el borde | Igual | Ídem |
| No detecta la línea blanca | `UMBRAL_QTR` mal calibrado | Recalibrar con test 03 |
| Las alas no se mueven | Pines o alimentación de los servos | Revisar el test 04 (servos en D4/D2) |
| Las alas fuerzan o zumban en un extremo | Un ángulo de `config.h` pasa del tope mecánico | Ajustar los cuatro `ANGULO_*` con el test 04 |
| No arranca nunca | Módulo ausente o polaridad mal | `START_MODO 0` (sin módulo) o el valor del **test 05** (`1` activo bajo, `2` MicroStart) |
| Se calientan los motores | PWM muy alto con 6S directa | Bajar `PWM_MAX` o usar buck de 12 V |
