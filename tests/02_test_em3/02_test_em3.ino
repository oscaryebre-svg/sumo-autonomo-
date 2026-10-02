// ============================================================
//  TEST 02 — SENSORES EM-3 (oponente)
//  Imprime el estado de los 4 sensores: 1 = rival detectado.
//  Prueba uno por uno: pasa la mano a ~20 cm delante de cada
//  sensor y su LED azul debe encenderse.
//
//  Orden de las columnas (según el chasis):
//    [ala_izq(A4) central_izq(A5) central_der(D1) ala_der(D0)]
//  Confirma que cada sensor responde en SU columna.
// ============================================================
#define PIN_EM3_ALA_IZQ      A4
#define PIN_EM3_CENTRAL_IZQ  A5
#define PIN_EM3_CENTRAL_DER  1
#define PIN_EM3_ALA_DER      0

void setup() {
  pinMode(PIN_EM3_ALA_IZQ, INPUT);
  pinMode(PIN_EM3_CENTRAL_IZQ, INPUT);
  pinMode(PIN_EM3_CENTRAL_DER, INPUT);
  pinMode(PIN_EM3_ALA_DER, INPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 02 EM-3: [ala_izq central_izq central_der ala_der]"));
}

void loop() {
  Serial.print(F("["));
  Serial.print(digitalRead(PIN_EM3_ALA_IZQ));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_CENTRAL_IZQ));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_CENTRAL_DER));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_ALA_DER));
  Serial.println(F("]"));
  delay(100);
}
