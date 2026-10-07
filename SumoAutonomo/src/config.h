// ============================================================
//  config.h — TODOS los ajustes del robot en un solo lugar.
//  Cambia aquí los valores; NO hace falta tocar la lógica.
//  Los valores numéricos se sincronizan solos con la simulación
//  (simulacion/constantes.py lee este archivo).
// ============================================================
#pragma once

#include <Arduino.h>
#include "tipos.h"

// ===================== PINES =====================
// --- Motores (dato verificado de la placa) ---
#define PIN_MOTOR_IZQ_PWM   3     // D3
#define PIN_MOTOR_IZQ_DIR   12    // D12
#define PIN_MOTOR_DER_PWM   11    // D11
#define PIN_MOTOR_DER_DIR   13    // D13

// --- Mini QTR: sensores de piso (ANALÓGICOS, 0..1023) ---
#define PIN_QTR_IZQ   A1   // A1 = suelo izquierdo
#define PIN_QTR_DER   A2   // A2 = suelo derecho

// --- EM-3: sensores de oponente (DIGITALES, 1 = rival detectado) ---
// Nombres según el chasis: "ala" = sensor del extremo, "central" = junto al centro.
#define PIN_EM3_ALA_IZQ      0    // D0, ala izquierda (es RX del Serial1)
#define PIN_EM3_CENTRAL_IZQ  1    // D1, central izquierdo (es TX del Serial1: no usar Serial1)
#define PIN_EM3_CENTRAL_DER  A5   // A5, central derecho
#define PIN_EM3_ALA_DER      A4   // A4, ala derecha

// --- Servos de las alas (MOT-110) ---
// Se controlan con la librería Servo (incluida en el IDE de Arduino).
#define PIN_ALA_IZQ   2     // D2 = servo ala izquierda
#define PIN_ALA_DER   4     // D4 = servo ala derecha

// --- Módulo de arranque (opcional) ---
// Pin del módulo de arranque. En la XMotion (placa completa) es D10, el
// mismo pin del botón de start. Compruébalo con el test 05.
#define PIN_START         10    // D10

// Cómo se recibe la señal de arranque. Elige UNA opción:
//   START_MODO 0 -> SIN MÓDULO: el robot arranca solo al encender.
//   START_MODO 1 -> módulo ACTIVO BAJO: arranca cuando el pin = LOW
//                   (pulsador/botón con resistencia de pull-up).
//   START_MODO 2 -> módulo ACTIVO ALTO: arranca cuando el pin = HIGH.
//                   Es el caso del JSumo MicroStart: en reposo da 0 V y al
//                   dar la señal pasa a 5 V (Logic 1) y se mantiene. POR DEFECTO.
#define START_MODO  2

// ===================== MOTORES =====================
// PWM máximo: limita el voltaje que reciben los motores de 6 V.
//   Batería 6S (22,2 V) directa -> 70  (27 % ≈ 6 V efectivos)
//   Con regulador buck de 12 V -> 255 (100 %)
#define PWM_MAX  70

// Sentido de avance de cada motor. Si un motor gira al revés en el
// test 01, cambia su valor de 1 a 0 (o de 0 a 1).
#define DIR_ADELANTE_IZQ  1
#define DIR_ADELANTE_DER  1

// ===================== SENSORES =====================
// Umbral del QTR (0..1023) para detectar la línea blanca.
// Valores medidos en el robot real (test 03):
//   negro (dohyo): izq 745, der 723
//   blanco (linea): izq 416, der 41
// Un solo umbral que separa los cuatro valores (entre 416 y 723) -> 570.
#define UMBRAL_QTR  570
// ¿La lectura del QTR es ALTA (1) o BAJA (0) sobre la línea BLANCA?
//   En este robot el blanco da un valor MENOR que el negro -> 0.
#define QTR_BORDE_ES_BLANCO  0

// ===================== ESTRATEGIA =====================
// Velocidades: van de 0 a PWM_MAX. El signo lo pone la lógica.
#define VEL_BUSQUEDA      40         // giro en el sitio buscando al rival
#define VEL_ATAQUE        PWM_MAX    // empuje a tope
#define VEL_ARCO_LENTO    25         // rueda interior al girar hacia el rival
#define VEL_ARCO_RAPIDO   PWM_MAX    // rueda exterior: avanza y gira a la vez
#define VEL_GIRO_BORDE    50         // giro de la maniobra de escape
#define VEL_RETROCESO     55         // retroceso al ver el borde

// Tiempos en milisegundos
#define TIEMPO_RETROCESO       180   // retroceder al ver el borde
#define TIEMPO_GIRO_BORDE      420   // girar tras el retroceso
#define TIEMPO_MEMORIA_LADO    800   // seguir girando hacia el último lado visto

// Duración del round: al cumplirse, el robot se detiene y recoge las alas.
#define TIEMPO_COMBATE_MS      180000  // 3 minutos (ajustar a las reglas)

// ===================== ALAS (ángulos del robot real) =====================
// Servo MOT-110 (Steren): analógico, 180°, 3,5-6 V, 40 mA. Se controlan
// con la librería Servo (viene incluida en el IDE): ella genera los pulsos
// por temporizador y el ala se mueve a la máxima velocidad del servo.
//
// Las dos alas comparten la posición de recogida en 90°. Van montadas
// ESPEJADAS, así que cada una se despliega 90° en sentido contrario:
//   izquierda: 90° (recogida) ->   0° (desplegada) = 90° (decreciente)
//   derecha:   90° (recogida) -> 180° (desplegada) = 90° (creciente)
// Recogida = inicio/medida y final del round; desplegada = durante el combate.
// Si en el test 04 el ala derecha girase al revés, intercambiar las dos parejas.
#define ANGULO_IZQ_RECOGIDA     90   // ala izquierda pegada al cuerpo
#define ANGULO_IZQ_DESPLIEGUE    0   // ala izquierda extendida
#define ANGULO_DER_RECOGIDA     90   // ala derecha pegada al cuerpo
#define ANGULO_DER_DESPLIEGUE  180   // ala derecha extendida

// ===================== DEPURACIÓN =====================
#define DEBUG_SERIAL  1   // 1 = imprime estados por USB (Serial Monitor a BAUDRATE)
#define BAUDRATE      115200
