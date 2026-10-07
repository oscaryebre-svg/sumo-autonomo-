// ============================================================
//  TEST 05 — MÓDULO DE ARRANQUE (polaridad)
//
//  Muestra por el Serial el nivel del pin del módulo para saber:
//   1. En qué pin responde (por defecto se lee D8).
//   2. Su polaridad: qué valor tiene EN REPOSO y qué valor da AL DAR
//      la señal de start.
//
//  Cómo usarlo:
//   1. Sube este test y abre el Monitor Serie a 115200 baud.
//   2. Anota el valor EN REPOSO (sin dar la señal).
//   3. Da la señal de start con el mando y anota el NUEVO valor.
//   4. Compara:
//        reposo 0  ->  señal 1   =>  START_MODO 2  (JSumo MicroStart)
//        reposo 1  ->  señal 0   =>  START_MODO 1  (pulsador con pull-up)
//   5. Si el valor no cambia nunca, el módulo no está en D8: prueba el
//      pin donde vaya conectado y anótalo.
// ============================================================
#define PIN_START 8

void setup() {
  pinMode(PIN_START, INPUT);
  Serial.begin(115200);
  Serial.println(F("TEST 05 ARRANQUE: lecturas del pin del modulo"));
  Serial.println(F("  anota el valor en reposo y el valor al dar la senal"));
  Serial.println(F("  reposo 0 -> senal 1  => START_MODO 2 (MicroStart)"));
  Serial.println(F("  reposo 1 -> senal 0  => START_MODO 1 (pulsador)"));
}

void loop() {
  Serial.println(digitalRead(PIN_START));
  delay(200);
}
