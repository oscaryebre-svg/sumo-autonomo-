// ============================================================
//  TEST 03 — MINI QTR (sensores de piso)
//  Imprime los valores analógicos de los 2 QTR (0..1023).
//
//  Calibración:
//   1. Pon el robot sobre el piso NEGRO del dohyo y anota los valores.
//   2. Pon cada sensor sobre la LÍNEA BLANCA y anota los valores.
//   3. Reporta ambos pares de números al desarrollador, que fijará
//      UMBRAL_QTR = (negro + blanco) / 2  en config.h.
//  (Lo normal: negro ≈ 300, blanco ≈ 800)
// ============================================================
#define PIN_QTR_IZQ A1
#define PIN_QTR_DER A2

void setup() {
  Serial.begin(115200);
  Serial.println(F("TEST 03 QTR: valores izq,der (negro ~300, blanco ~800)"));
}

void loop() {
  Serial.print(analogRead(PIN_QTR_IZQ));
  Serial.print(F(","));
  Serial.println(analogRead(PIN_QTR_DER));
  delay(100);
}
