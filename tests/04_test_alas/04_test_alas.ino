// ============================================================
//  TEST 04 — ALAS (servos MOT-110 en D4 izquierda y D2 derecha)
//
//  Mueve cada ala por separado, despacio, de 0° a 180° y de vuelta,
//  imprimiendo el ángulo. Usa la librería Servo (incluida en el IDE).
//
//  Sirve para:
//   1. Ver el recorrido real de cada ala (debe ser suave, sin forzar).
//   2. Confirmar los ángulos RECOGIDA y DESPLEGADA de cada ala. En el
//      robot real ya verificado son:
//        izquierda: recogida 120° · desplegada  10°
//        derecha:   recogida  75° · desplegada 175°
//      Van montadas espejadas, por eso no coinciden ni recorren 90°.
//   3. Comprobar que el ala derecha gira espejada (al revés que la
//      izquierda).
//
//  Al final del barrido lleva las dos alas a esas posiciones
//  verificadas para que las confirmes a simple vista.
// ============================================================
#include <Servo.h>

#define PIN_ALA_IZQ 4
#define PIN_ALA_DER 2

// Ángulos verificados en el robot: deben coincidir con config.h.
#define IZQ_RECOGIDA     120
#define IZQ_DESPLEGADA    10
#define DER_RECOGIDA      75
#define DER_DESPLEGADA   175

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

// Lleva las dos alas a las posiciones verificadas (recogida y desplegada)
// para comprobar que son las correctas.
void posicionesVerificadas() {
  Serial.println(F("--- verificadas: RECOGIDAS ---"));
  IZservo.write(IZQ_RECOGIDA);
  Drservo.write(DER_RECOGIDA);
  delay(1500);
  Serial.println(F("--- verificadas: DESPLEGADAS ---"));
  IZservo.write(IZQ_DESPLEGADA);
  Drservo.write(DER_DESPLEGADA);
  delay(1500);
}

void setup() {
  IZservo.attach(PIN_ALA_IZQ);
  Drservo.attach(PIN_ALA_DER);
  Serial.begin(115200);
  Serial.println(F("TEST 04 ALAS: barrido lento 0..180..0"));
  Serial.println(F("Anota RECOGIDA y DESPLEGADA de cada ala (van espejadas, no 90)."));
}

void loop() {
  Serial.println(F("--- ala IZQUIERDA (D4) ---"));
  barrido(IZservo, "IZQ");
  Serial.println(F("--- ala DERECHA (D2) ---"));
  barrido(Drservo, "DER");
  Serial.println(F("--- alas en las posiciones verificadas ---"));
  posicionesVerificadas();
}
