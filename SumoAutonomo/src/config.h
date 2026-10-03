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
#define PIN_QTR_IZQ   A1
#define PIN_QTR_DER   A2

// --- EM-3: sensores de oponente (DIGITALES, 1 = rival detectado) ---
// Nombres según el chasis: "ala" = sensor del extremo, "central" = junto al centro.
#define PIN_EM3_ALA_IZQ      A4   // sensor de ala izquierda
#define PIN_EM3_CENTRAL_IZQ  A5   // sensor central izquierdo
#define PIN_EM3_CENTRAL_DER  1    // D1, central derecho (es TX del Serial1: no usar Serial1)
#define PIN_EM3_ALA_DER      0    // D0, ala derecha (es RX del Serial1)

// --- Servos de las alas (MOT-110) ---
// Se controlan con la librería Servo (incluida en el IDE de Arduino).
#define PIN_ALA_DER   2     // D2 = servo derecho
#define PIN_ALA_IZQ   4     // D4 = servo izquierdo

// --- Módulo de arranque (opcional) ---
#define PIN_START         8
#define START_ACTIVO_BAJO 1   // 1 = el módulo da LOW al arrancar (estilo MicroStart)
                               // 0 = el robot corre al encender si no hay módulo

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
// Se calibra con tests/03: UMBRAL = (valor_negro + valor_blanco) / 2
#define UMBRAL_QTR  500
// 1 = lectura ALTA sobre la línea blanca (lo normal en estos sensores)
#define QTR_BORDE_ES_BLANCO  1

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

// ===================== ALAS (90° de recorrido) =====================
// Servo MOT-110 (Steren): analógico, 180°, 3,5-6 V, 40 mA. Se controlan
// con la librería Servo (viene incluida en el IDE): ella genera los pulsos
// por temporizador, así que el bucle no tiene que mantenerlos y el ala se
// mueve a la máxima velocidad del servo (< 0,1 s para 90°).
//
// Cada ala tiene DOS posiciones y sus propios ángulos (van montadas
// espejadas): recogida (inicio/medida y final del round) y desplegada
// (combate, para engañar a los sensores). Entre ambas hay 90° de recorrido.
// Los valores de recogida vienen del código que ya funciona; se ajustan
// con el test 04.
#define ANGULO_IZQ_RECOGIDA     120   // ala izquierda pegada al cuerpo
#define ANGULO_IZQ_DESPLIEGUE    30   // 120 - 90 = 30
#define ANGULO_DER_RECOGIDA      75   // ala derecha pegada al cuerpo
#define ANGULO_DER_DESPLIEGUE   165   // 75 + 90 = 165

// ===================== DEPURACIÓN =====================
#define DEBUG_SERIAL  1   // 1 = imprime estados por USB (Serial Monitor a BAUDRATE)
#define BAUDRATE      115200
