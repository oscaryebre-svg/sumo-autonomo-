// ============================================================
//  TEST 02 — SENSORES EM-3 (oponente)
//  Imprime el estado de los 4 sensores: 1 = rival detectado.
//  Prueba uno por uno: pasa la mano a ~20 cm delante de cada
//  sensor y su LED azul debe encenderse.
//
//  Orden de las columnas (según el chasis):
//    [ala_izq(D0) central_izq(D1) central_der(A5) ala_der(A4)]
//  Confirma que cada sensor responde en SU columna.
// ============================================================
#define PIN_EM3_ALA_IZQ      0
#define PIN_EM3_CENTRAL_IZQ  1
#define PIN_EM3_CENTRAL_DER  A5
#define PIN_EM3_ALA_DER      A4

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
