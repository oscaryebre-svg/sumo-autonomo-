// ============================================================
//  alas.h — control de los 2 servos MOT-110 (alas retráctiles)
//
//  Las alas SOLO sirven para engañar a los sensores del rival.
//  Tienen 90° de recorrido: recogidas (para medir/entrar) y
//  desplegadas (durante el combate). No hay más movimientos.
//
//  Los pines (D2 y D4) no son de temporizador, así que los pulsos
//  se generan por software: NO se usa la librería Servo.
// ============================================================
#pragma once
#include <Arduino.h>
#include "config.h"

// Configura los pines y las deja RECOGIDAS (llamar en setup)
void alas_init();

// Despliega las alas para engañar a los sensores del rival
void alas_desplegar();

// Vuelve a recogerlas
void alas_recoger();

// Llamar en CADA vuelta del loop: mantiene la señal de los servos
void alas_actualizar();
