// ============================================================
//  TEST 04 — ALAS (servos MOT-110 en D4 izquierda y D2 derecha)
//
//  Mueve cada ala por separado, despacio, de 0° a 180° y de vuelta,
//  imprimiendo el ángulo. Usa la librería Servo (incluida en el IDE).
//
//  Sirve para:
//   1. Ver el recorrido real de cada ala (debe ser suave, sin forzar).
//   2. Anotar el ángulo RECOGIDA y el ángulo DESPLEGADA de cada ala,
//      separados por 90° (esos cuatro números van a config.h).
//   3. Ver si el ala derecha va espejada (al revés que la izquierda).
// ============================================================
#include <Servo.h>

#define PIN_ALA_IZQ 4
#define PIN_ALA_DER 2

Servo IZservo, Drservo;

void barrido(Servo &s, const char *nombre) {
  for (int a = 0; a <= 180; a += 10) {
    s.write(a);
    Serial.print(nombre);
    Serial.print(F(" = "));
    Serial.println(a);
    delay(250);
  }
  for (int a = 180; a >= 0; a -= 10) {
    s.write(a);
    Serial.print(nombre);
    Serial.print(F(" = "));
    Serial.println(a);
    delay(250);
  }
}

void setup() {
  IZservo.attach(PIN_ALA_IZQ);
  Drservo.attach(PIN_ALA_DER);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS: barrido lento 0..180..0"));
  Serial.println(F("Anota el angulo RECOGIDA y DESPLEGADA de cada ala (90 entre ellos)."));
}

void loop() {
  Serial.println(F("--- ala IZQUIERDA (D4) ---"));
  barrido(IZservo, "IZQ");
  Serial.println(F("--- ala DERECHA (D2) ---"));
  barrido(Drservo, "DER");
}
