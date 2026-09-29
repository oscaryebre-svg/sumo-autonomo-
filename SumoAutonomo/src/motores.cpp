#include "motores.h"

// Aplica velocidad a UN motor: el signo va al pin DIR, la magnitud al pin PWM.
static void motor_set(uint8_t pinPwm, uint8_t pinDir,
                      int16_t vel, uint8_t dirAdelante) {
  int16_t v = constrain(vel, (int16_t)-PWM_MAX, (int16_t)PWM_MAX);
  digitalWrite(pinDir, v >= 0 ? dirAdelante : !dirAdelante);
  analogWrite(pinPwm, abs(v));
}

void motores_init() {
  pinMode(PIN_MOTOR_IZQ_PWM, OUTPUT);
  pinMode(PIN_MOTOR_IZQ_DIR, OUTPUT);
  pinMode(PIN_MOTOR_DER_PWM, OUTPUT);
  pinMode(PIN_MOTOR_DER_DIR, OUTPUT);
  motores_frenar();
}

void motores_set(int16_t izq, int16_t der) {
  motor_set(PIN_MOTOR_IZQ_PWM, PIN_MOTOR_IZQ_DIR, izq, DIR_ADELANTE_IZQ);
  motor_set(PIN_MOTOR_DER_PWM, PIN_MOTOR_DER_DIR, der, DIR_ADELANTE_DER);
}

void motores_frenar() {
  analogWrite(PIN_MOTOR_IZQ_PWM, 0);
  analogWrite(PIN_MOTOR_DER_PWM, 0);
}
