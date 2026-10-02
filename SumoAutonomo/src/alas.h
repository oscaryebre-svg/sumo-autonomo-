// ============================================================
//  alas.h — control de los 2 servos MOT-110 (alas retráctiles)
//
//  Las alas SOLO sirven para engañar a los sensores del rival.
//  Tienen 90° de recorrido: recogidas (para medir/entrar) y
//  desplegadas (durante el combate). No hay más movimientos.
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
