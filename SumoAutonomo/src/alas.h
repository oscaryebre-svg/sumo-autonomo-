// ============================================================
//  alas.h — control de los 2 servos MOT-110 (alas retráctiles)
//
//  Las alas SOLO sirven para engañar a los sensores del rival.
//  Van montadas espejadas (izquierda 90°/0°, derecha 90°/180°) y
//  tienen dos posiciones:
//    recogida   -> al inicio (medida) y al terminar el round
//    desplegada -> durante el combate
//
//  Se usa la librería Servo (incluida en el IDE), que genera los
//  pulsos por temporizador y mueve el ala a su máxima velocidad.
// ============================================================
#pragma once
#include <Arduino.h>
#include "config.h"

// Configura los servos y las deja RECOGIDAS (llamar en setup)
void alas_init();

// Despliega las alas para engañar a los sensores del rival
void alas_desplegar();

// Vuelve a recogerlas
void alas_recoger();
