// ============================================================
//  TEST 03 — MINI QTR (sensores de piso)
//  Imprime los valores analógicos de los 2 QTR (0..1023).
//
//  Calibración:
//   1. Pon el robot sobre el piso NEGRO del dohyo y anota los valores.
//   2. Pon cada sensor sobre la LÍNEA BLANCA y anota los valores.
//   3. Con esos cuatro números se fijan dos cosas en config.h:
//        UMBRAL_QTR = (negro + blanco) / 2
//        QTR_BORDE_ES_BLANCO = 1 si el BLANCO da un valor MAYOR que el negro,
//                              0 si el BLANCO da un valor MENOR que el negro.
//
//  OJO: en algunos sensores el blanco da MENOS valor que el negro. Fíjate en
//  cuál es mayor; si el robot confunde el blanco con el negro, cambia el flag.
// ============================================================
#define PIN_QTR_IZQ A1
#define PIN_QTR_DER A2

void setup() {
  Serial.begin(115200);
  Serial.println(F("TEST 03 QTR: anota izq,der sobre NEGRO y sobre BLANCO"));
  Serial.println(F("Fijate si sobre BLANCO el valor es mayor o menor que NEGRO."));
}

void loop() {
  Serial.print(analogRead(PIN_QTR_IZQ));
  Serial.print(F(","));
  Serial.println(analogRead(PIN_QTR_DER));
  delay(100);
}
