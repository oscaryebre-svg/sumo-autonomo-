# Tutorial para el Tester — Robot de Sumo Autónomo

Este documento es para la persona que tiene el robot físicamente.
Aquí está TODO lo que necesitas hacer, sin saber programar:
instalar, subir el código, calibrar y **documentar los resultados**.
Al final tienes una **explicación del código** (sección 8) por si quieres
entender qué hace el robot por dentro.

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
   - **Windows:** instala el `.exe` como cualquier programa. Si aparece un
     aviso azul de **SmartScreen**, pulsa *Más información* → *Ejecutar de
     todas formas*.
   - **Linux:** descarga el AppImage; haz clic derecho → *Permisos* →
     marca *Ejecutable* → doble clic para abrirlo.
3. Abre el Arduino IDE una vez para que cree sus carpetas.

**No hay que instalar nada:** la única librería que se usa (`Servo`) viene
incluida en el IDE de Arduino.

## 2. Conectar el robot

1. Conecta el cable **micro-USB** de la placa a la computadora.
2. En el IDE: menú **Tools → Port** → elige el puerto que aparezca
   (`COM3`, `COM4`… en Windows; `/dev/ttyACM0` en Linux).
3. Menú **Tools → Board**:
   - Si la placa dice **XMotion** → elige **Arduino Leonardo**.
   - Si la placa dice **Genesis/M1** → elige **Arduino Nano**.
   - Si no estás seguro, mira la serigrafía de la placa y anótala en el reporte.

> **Windows:** si en **Tools → Port** no aparece ningún `COM…`, instala el
> driver USB de la placa y vuelve a conectarla. Para clones estilo **Nano**
> suele hacer falta el driver **CH340**; las placas **Leonardo/XMotion** no
> lo necesitan.

## 3. Obtener el código y subirlo al robot

### 3.1 Descargar el código desde GitHub

El código está en <https://github.com/oscaryebre-svg/sumo-autonomo->, en la
rama **main** (la que se abre por defecto).

**En Windows (paso a paso):**

1. Abre el enlace y pulsa el botón verde **Code** → **Download ZIP**.
2. Se descarga un archivo `.zip` (normalmente en **Descargas**).
3. Clic derecho sobre el `.zip` → **Extraer todo…** → **Extraer**.
4. Se crea una carpeta con el nombre repetido dos veces. Entra hasta que
   veas dentro la carpeta **`SumoAutonomo`** (y no la muevas ni la renombres:
   dentro tiene que quedar la subcarpeta **`src`**).

> Si no descomprimes el ZIP, o borras la carpeta `src`, el IDE de Arduino no
> encontrará los archivos.

**Otros sistemas / con Git:** también puedes clonar la rama `main` del
repositorio y abrir la carpeta `SumoAutonomo`.

### 3.2 Subir el programa principal

1. Menú **File → Open** → busca la carpeta `SumoAutonomo` y abre
   **`SumoAutonomo.ino`** (⚠️ no renombres la carpeta ni el archivo).
   En Windows la ruta será parecida a
   `Descargas\...\SumoAutonomo\SumoAutonomo.ino`.
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

### Test 04 — Alas (servos MOT-110) (`tests/04_test_alas`)

El **MOT-110** es un servo analógico de 180°. Este test mueve **cada ala por
separado**, despacio, de 0° a 180°, e imprime el ángulo. Están en los pines
**D4 (izquierda)** y **D2 (derecha)** y usan la librería **Servo**, que ya
viene incluida en el IDE: no hay que instalar nada.

**Pasos:**
1. Observa el barrido de cada ala: debe ser **suave, sin tirones ni pausas**.
2. Anota el **ángulo en el que el ala está totalmente RECOGIDA** (pegada al
   cuerpo) y el **ángulo en el que está totalmente DESPLEGADA**. Si fuerza
   o zumba en un extremo, ese ángulo ya pasa del tope: anótalo.
3. ¿El ala derecha gira "espejada" (al revés que la izquierda)?

**Anota:** los dos ángulos de cada ala (recogida y desplegada), si el
recorrido es suave y si van espejadas. Con eso se ajustan los cuatro
`ANGULO_IZQ_*` / `ANGULO_DER_*` en `config.h`.

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

TEST 04 — ALAS (servo MOT-110, barrido de 0° a 180°)
- ¿El movimiento es suave (sin tirones ni pausas)? ( ) sí  ( ) no
- Ala IZQUIERDA: recogida ____° · desplegada ____°
- Ala DERECHA:   recogida ____° · desplegada ____°
- ¿Fuerza o zumba en algún extremo? ( ) no ( ) izq en ____° ( ) der en ____°
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
| No aparece ningún puerto `COM` (Windows) | Falta el driver USB: instala el **CH340** y reconecta |
| `Servo.h: No such file or directory` | Instala la librería **Servo** desde **Library Manager** |
| `config.h: No such file or directory` | Descarga otra vez el ZIP y no borres la carpeta `src` |
| Sube pero el robot no se mueve | Sin módulo de arranque es normal; repórtalo |
| No detecta la línea blanca | Revisa altura del sensor y repite el test 03 |
| Se sale del dohyo | Repórtalo con el video de la situación |
| Motores muy lentos o muy agresivos | Repórtalo; se ajusta en `config.h` |
| Las alas fuerzan o zumban en un extremo | Repórtalo con el ángulo (test 04); se ajustan los cuatro `ANGULO_*` |

**Regla de oro:** si algo se ve raro, **grábalo 10 segundos y pásalo**:
vale más un video que diez descripciones.

## 8. Explicación del código (para entender qué subes)

> Esta sección es informativa: no necesitas programar para hacer los tests.
> Sirve para que entiendas qué hace el robot por dentro.

### 8.1 Qué hace cada archivo

| Archivo | En una frase |
|---|---|
| `SumoAutonomo/SumoAutonomo.ino` | Programa principal: la secuencia del round (esperar → pelear → parar). |
| `SumoAutonomo/src/config.h` | Los números ajustables: pines, velocidades, tiempos y ángulos de las alas. |
| `SumoAutonomo/src/estrategia.cpp` | El cerebro: decide el movimiento en cada instante. |
| `SumoAutonomo/src/motores.cpp` | Da marcha y velocidad a las 2 ruedas. |
| `SumoAutonomo/src/sensores.cpp` | Lee los 4 sensores de rival y los 2 del piso. |
| `SumoAutonomo/src/alas.cpp` | Mueve los 2 servos de las alas. |
| `SumoAutonomo/src/tipos.h` | Los "sobres" de datos que se pasan los módulos. |
| `tests/01_test_motores` … `tests/04_test_alas` | Los 4 tests de calibración. |

Ninguna carpeta ni archivo debe renombrarse: el IDE de Arduino los busca
por su nombre.

### 8.2 La vuelta del programa (el bucle)

El robot repite sin parar este ciclo, muchísimas veces por segundo:

1. **Lee** los sensores (rival y piso).
2. **Decide** (la estrategia) si toca buscar, atacar o escapar.
3. **Mueve** las ruedas según esa decisión.

Además, en el Serial Monitor imprime una línea cada 0,1 s para que veas qué
está "pensando" en cada momento.

### 8.3 La estrategia: la misma pregunta, por orden de importancia

En cada vuelta, el robot mira las cosas en este orden:

```
1º ¿Toco la línea blanca?  ->  ESCAPAR del borde   (lo más importante)
2º ¿Veo al rival?          ->  ATACAR
3º No veo nada             ->  BUSCAR girando
```

- **ESCAPAR:** si un sensor del piso ve el blanco, primero **retrocede
  180 ms** y luego **gira 420 ms**. Cada vez gira hacia el lado contrario
  del escape anterior, para no quedarse atrapado en una esquina. Mientras
  escapa **ignora al rival** (por eso va primero).
- **ATACAR:** si un sensor de rival lo detecta:
  - si lo tiene **de frente** → empuja con las dos ruedas a tope;
  - si lo tiene **a un lado** → hace un arco (una rueda lenta y otra
    rápida) para **girar y avanzar a la vez**, cerrándole la distancia.
- **BUSCAR:** si no ve a nadie, **gira despacio sobre sí mismo** para
  barrer los 360° del dohyo. Si acaba de perder al rival, durante 0,8 s
  sigue girando hacia el último lado donde lo vio.

Esta lógica es exactamente la que cuenta el campo `est=` del Serial
Monitor: **0** buscar, **1** atacar, **2** escapar.

### 8.4 Los sentidos del robot

- **4 sensores de rival (EM-3)**, digitales: `1` = rival delante. Hay dos a
  los lados (las "alas") y dos junto al centro; así distingue si el rival
  está de frente o de lado.
- **2 sensores de piso (Mini QTR)**, analógicos (0–1023): uno a cada lado.
  Sobre la línea blanca dan un valor alto; sobre el negro del dohyo, bajo.
  El robot los usa para no salirse.

### 8.5 Las alas

Los 2 servos **no empujan al rival**: solo sirven para **engañar a sus
sensores**. Se **recogen** (90°, pegadas al cuerpo) antes de empezar y al
terminar el round, y se **despliegan** (una a 0° y la otra a 180°, 90° cada
una) durante el combate. Van montadas espejadas.

### 8.6 Dónde se ajusta todo

Casi todo lo que se puede cambiar está en `SumoAutonomo/src/config.h`:
pines, velocidades, tiempos y los ángulos de las alas. Por eso el
desarrollador solo toca ese archivo con los datos que tú anotes en el
reporte.
