// ============================================================
//  TEST 02 — SENSORES EM-3 (oponente)
//  Imprime el estado de los 4 sensores: 1 = rival detectado.
//  Prueba uno por uno: apunta el sensor a un objeto a 20-40 cm
//  (o tu mano) y su LED azul debe encenderse.
//
//  Reporta al desarrollador qué sensor físico corresponde a cada
//  columna [EM3_1 EM3_2 EM3_3 EM3_4], de izquierda a derecha
//  mirando el robot de frente. Con eso se ordena config.h.
// ============================================================
#define PIN_EM3_1 A4
#define PIN_EM3_2 A5
#define PIN_EM3_3 1
#define PIN_EM3_4 0

void setup() {
  pinMode(PIN_EM3_1, INPUT);
  pinMode(PIN_EM3_2, INPUT);
  pinMode(PIN_EM3_3, INPUT);
  pinMode(PIN_EM3_4, INPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 02 EM-3: columnas = [EM3_1 EM3_2 EM3_3 EM3_4]"));
}

void loop() {
  Serial.print(F("["));
  Serial.print(digitalRead(PIN_EM3_1));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_2));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_3));
  Serial.print(F(" "));
  Serial.print(digitalRead(PIN_EM3_4));
  Serial.println(F("]"));
  delay(100);
}
