// ============================================================
//  TEST 04 — ALAS (servos MOT-110 en D2 izquierda y D4 derecha)
//
//  Mueve las dos alas entre su posición RECOGIDA (90° en las dos) y su
//  posición DESPLEGADA (izquierda 0°, derecha 180°). Van montadas
//  espejadas: cada una recorre 90° en sentido contrario.
//
//  El movimiento se pide de golpe (solo se escribe el ángulo destino), así
//  que el servo va a su MÁXIMA velocidad disponible, sin pasos intermedios
//  ni pausas. Usa la librería Servo (incluida en el IDE).
//
//  Sirve para:
//   1. Ver que cada ala se mueve rápido y suave, sin forzar.
//   2. Confirmar el recorrido de 90°: izquierda 90°→0°, derecha 90°→180°.
//   3. Comprobar que la derecha gira espejada (al revés que la izquierda).
// ============================================================
#include <Servo.h>

#define PIN_ALA_IZQ 2
#define PIN_ALA_DER 4

// Ángulos del robot: deben coincidir con config.h.
#define IZQ_RECOGIDA     90
#define IZQ_DESPLEGADA    0
#define DER_RECOGIDA     90
#define DER_DESPLEGADA  180

Servo IZservo, Drservo;

void setup() {
  IZservo.attach(PIN_ALA_IZQ);
  Drservo.attach(PIN_ALA_DER);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS: recogida 90/90 <-> desplegada 0/180"));
  Serial.println(F("Movimiento directo, a la maxima velocidad del servo."));
}

void loop() {
  Serial.println(F("RECOGIDAS (90, 90)"));
  IZservo.write(IZQ_RECOGIDA);     // 90
  Drservo.write(DER_RECOGIDA);     // 90
  delay(1500);                     // solo para verlas quietas en el tope

  Serial.println(F("DESPLEGADAS (0, 180)"));
  IZservo.write(IZQ_DESPLEGADA);   // 0
  Drservo.write(DER_DESPLEGADA);   // 180
  delay(1500);
}
