# Tutorial para el Tester — Robot de Sumo Autónomo

Este documento es para la persona que tiene el robot físicamente.
Aquí está TODO lo que necesitas hacer, sin saber programar:
instalar, subir el código, calibrar y **documentar los resultados**.

> ⚠️ **Seguridad de la batería (leer primero)**
> - La batería es **LiPo 6S de 22,2 V**. No la perfore, no la cortocircuite y
>   cárgala solo con un cargador LiPo balanceado.
> - **No conectes la batería de 22,2 V directo a la placa** si esta admite
>   máximo 15 V: usa el **regulador reductor (buck) de 12 V** entre la batería
>   y la placa.
> - Usa una **alarma LiPo** en el conector de balance durante las pruebas.

---

## 1. Instalar el Arduino IDE

1. Entra a <https://www.arduino.cc/en/software>.
2. Descarga la versión 2.x para tu sistema:
   - **Windows:** instala el `.exe` como cualquier programa.
   - **Linux:** descarga el AppImage; haz clic derecho → *Permisos* →
     marca *Ejecutable* → doble clic para abrirlo.
3. Abre el Arduino IDE una vez para que cree sus carpetas.

**No hay que instalar ninguna librería:** el código no las necesita.

## 2. Conectar el robot

1. Conecta el cable **micro-USB** de la placa a la computadora.
2. En el IDE: menú **Tools → Port** → elige el puerto que aparezca
   (`COM3`, `COM4`… en Windows; `/dev/ttyACM0` en Linux).
3. Menú **Tools → Board**:
   - Si la placa dice **XMotion** → elige **Arduino Leonardo**.
   - Si la placa dice **Genesis/M1** → elige **Arduino Nano**.
   - Si no estás seguro, mira la serigrafía de la placa y anótala en el reporte.

## 3. Subir el programa principal

1. Menú **File → Open** → busca la carpeta `SumoAutonomo` y abre
   **`SumoAutonomo.ino`** (⚠️ no renombres la carpeta ni el archivo).
2. Pulsa el botón **Upload (→)** (flecha, arriba a la izquierda).
3. Abajo debe terminar en `Done uploading` sin errores.
4. Abre **Tools → Serial Monitor** y en la esquina inferior derecha
   elige **115200 baud**. Verás `Sumo autonomo: listo`.

### Qué significan las líneas del Serial Monitor

El robot imprime una línea cada 0,1 s:

```
12.3s est=1 qtr=340,355 em3=0100 mot=70,70
```

| Campo | Significado |
|---|---|
| `12.3s` | Segundos desde el arranque del combate |
| `est=` | Estado: **0** buscando · **1** atacando · **2** escapando del borde |
| `qtr=` | Lectura de los 2 sensores de piso (bajo = negro, alto = blanco) |
| `em3=` | Los 4 sensores de oponente (0 = nada, 1 = rival detectado) |
| `mot=` | Velocidad pedida a cada motor (positivo = adelante) |

> Si el robot **no se mueve nunca** y no tienes **módulo de arranque**,
> es normal: está esperando la señal. Anótalo en el reporte y el
> desarrollador lo desactiva en un minuto.

## 4. Calibrar: los 4 tests

Cada test es un archivo aparte. Se sube igual que el principal
(**File → Open → el .ino del test → Upload**). No hace falta saber qué
hace por dentro; solo sigue las instrucciones y anota lo que pide.

### Test 01 — Motores (`tests/01_test_motores`)

El robot repite: adelante → reversa → giro izquierda → giro derecha → freno.
El Serial dice qué paso toca en cada momento.

**Anota:**
- ¿Avanza **derecho** o se tuerce? Si se tuerce, ¿cuál rueda va al revés?
- ¿Los giros son hacia el lado correcto?

### Test 02 — Sensores EM-3 (`tests/02_test_em3`)

Imprime 4 columnas `[ala_izq central_izq central_der ala_der]` (1 = detecta).
Cada sensor tiene un **LED azul** que se enciende al detectar.

**Pasos:**
1. Pasa la mano a ~20 cm delante de cada sensor, uno por uno, y comprueba
   que se enciende **su** columna:
   - sensor de ala izquierda (A4) → primera columna
   - sensor central izquierdo (A5) → segunda columna
   - sensor central derecho (D1) → tercera columna
   - sensor de ala derecha (D0) → cuarta columna
2. Aléjate despacio y anota a qué distancia deja de detectarte.

**Anota:** que cada sensor responde en su columna y el alcance aproximado.

### Test 03 — Sensores de piso Mini QTR (`tests/03_test_qtr`)

Imprime dos números (0–1023): lectura izquierda y derecha del piso.

**Pasos:**
1. Con el robot sobre el piso **negro** del dohyo, anota los dos números.
2. Pon los sensores sobre la **línea blanca** y anota los dos números.
3. Anota la altura del sensor al piso (mm).

### Test 04 — Alas (servos) (`tests/04_test_alas`)

Las alas se mueven **despacio y de forma continua** entre la posición
recogida y la desplegada (90° de recorrido). El Serial imprime el pulso en
microsegundos y el ángulo equivalente. Están en los pines **D4
(izquierda)** y **D2 (derecha)**; los pulsos se generan por software, así
que no hay que instalar ninguna librería.

**Pasos:**
1. Observa el barrido completo: debe ser **suave, sin tirones ni pausas**.
2. Si el ala se atasca, zumba o choca con el chasis, **anota el µs** que
   marca el Serial en ese momento (ese es el límite mecánico).
3. ¿El ala derecha se mueve "espejada" (al revés que la izquierda)?

**Anota:** si el recorrido es suave, el µs donde se atasca y si la derecha
va espejada. Con eso se ajustan `PULSO_ALA_RECOGIDA` y
`PULSO_ALA_DESPLIEGUE` en `config.h`.

## 5. Formulario de reporte (cópialo y rellénalo)

```text
DATOS GENERALES
- Qué dice la serigrafía de la placa: ______________
- Placa elegida en el IDE: ______________
- Alimentación usada: ( ) buck 12 V   ( ) batería directa
- Versión del código probado: ______________

TEST 01 — MOTORES
- Avanza derecho: ( ) sí  ( ) no
- Motor al revés: ( ) izquierdo  ( ) derecho  ( ) ninguno
- Giros correctos: ( ) sí  ( ) no
- Observaciones: ______________

TEST 02 — EM-3 (¿responde cada sensor en su columna?)
- Ala izquierda (A4):     ( ) sí  ( ) no
- Central izquierdo (A5): ( ) sí  ( ) no
- Central derecho (D1):   ( ) sí  ( ) no
- Ala derecha (D0):       ( ) sí  ( ) no
- Alcance aproximado (cm): ____

TEST 03 — MINI QTR
- Sobre negro:  izq = ____   der = ____
- Sobre blanco: izq = ____   der = ____
- Altura del sensor al piso (mm): ____

TEST 04 — ALAS (recorrido 90°: pulso de 1000 a ~1500 µs)
- ¿El movimiento es suave (sin tirones ni pausas)? ( ) sí  ( ) no
- Si se atasca o zumba, ¿en qué µs? izq ____ µs · der ____ µs
- ¿Derecha espejada? ( ) sí  ( ) no

PROGRAMA PRINCIPAL (con Serial Monitor abierto)
- Pega 15-20 líneas del Serial en cada situación:
  a) al arrancar    b) buscando     c) frente al rival     d) en el borde
- ¿Se despliegan las alas al arrancar el combate? ( ) sí  ( ) no
- ¿Se quedan fijas durante el combate (sin vibrar)? ( ) sí  ( ) no
- ¿Se recogen al terminar el round (a los 3 min)? ( ) sí  ( ) no
- ¿Empuja al rival de frente? ( ) sí  ( ) no
- ¿Retrocede y gira al pisar la línea blanca? ( ) sí  ( ) no
- ¿Se sale del dohyo en algún momento? ( ) sí  ( ) no
- ¿Necesita módulo de arranque? ( ) sí  ( ) no

EXTRA (muy útil)
- Foto del robot armado: ( ) adjunta
- Video corto de cada test: ( ) adjunta
```

## 6. Cómo enviar el reporte

1. Copia las líneas del Serial Monitor (Ctrl+A → Ctrl+C) y pégalas en el
   formulario.
2. Envía el formulario junto con las fotos/videos al desarrollador.

## 7. Problemas comunes

| Síntoma | Qué hacer |
|---|---|
| `Error: port not found` | Revisa el cable y el puerto en Tools → Port |
| Sube pero el robot no se mueve | Sin módulo de arranque es normal; repórtalo |
| No detecta la línea blanca | Revisa altura del sensor y repite el test 03 |
| Se sale del dohyo | Repórtalo con el video de la situación |
| Motores muy lentos o muy agresivos | Repórtalo; se ajusta en `config.h` |
| Las alas dan tirones o zumban | Repórtalo con el µs donde empieza (test 04); se ajustan los pulsos |

**Regla de oro:** si algo se ve raro, **grábalo 10 segundos y pásalo**:
vale más un video que diez descripciones.
