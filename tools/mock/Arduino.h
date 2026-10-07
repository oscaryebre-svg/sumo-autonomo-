// Mock mínimo de Arduino para compilar la lógica en PC sin la placa.
// Solo define lo que usan los módulos de src/.
#pragma once
#include <stdint.h>
#include <stdlib.h>

#define HIGH 1
#define LOW  0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Etiquetas de pines del Arduino Leonardo (ATmega32U4)
#define A0 18
#define A1 19
#define A2 20
#define A3 21
#define A4 22
#define A5 23

#define constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// ---- Reloj controlable desde los tests ----
inline unsigned long &microsActual() { static unsigned long t = 0; return t; }
inline unsigned long micros() { return microsActual(); }
inline unsigned long millis() { return 0; }
inline void delay(unsigned long) {}
inline void delayMicroseconds(unsigned int) {}

// ---- Registro de transiciones de pines (para medir los pulsos) ----
struct RegistroPines {
  static const int MAX = 512;
  int n;
  uint8_t pin[MAX];
  uint8_t nivel[MAX];
  unsigned long t[MAX];
};
inline RegistroPines &registro() { static RegistroPines r = {0, {0}, {0}, {0}}; return r; }

inline void pinMode(uint8_t, uint8_t) {}

inline void digitalWrite(uint8_t p, uint8_t v) {
  RegistroPines &r = registro();
  if (r.n < RegistroPines::MAX) {
    r.pin[r.n] = p;
    r.nivel[r.n] = v;
    r.t[r.n] = micros();
    r.n++;
  }
}

// ---- Entradas digitales controlables desde los tests ----
// digitalRead() devuelve el nivel guardado con nivelPin() (por defecto 0 = LOW).
inline int &nivelPin(uint8_t p) { static int v[32] = {0}; return v[p & 31]; }
inline int digitalRead(uint8_t p) { return nivelPin(p); }

// ---- Salidas PWM registradas (para comprobar los motores) ----
inline int &pwmPin(uint8_t p) { static int v[32] = {0}; return v[p & 31]; }
inline void analogWrite(uint8_t p, int v) { pwmPin(p) = v; }

// ---- Entradas analógicas controlables (sensores de piso) ----
inline int &analogPin(uint8_t p) { static int v[32] = {0}; return v[p & 31]; }
inline int analogRead(uint8_t p) { return analogPin(p); }

// ---- Serial mínimo (solo para poder compilar el .ino en PC) ----
class PrintMock {
public:
  void begin(unsigned long) {}
  void print(const char *) {}
  void print(char) {}
  void print(unsigned char) {}
  void print(int) {}
  void print(unsigned int) {}
  void print(long) {}
  void print(unsigned long) {}
  void print(float, int = 2) {}
  void print(double, int = 2) {}
  void println(const char *) {}
  void println(char) {}
  void println(unsigned char) {}
  void println(int) {}
  void println(unsigned int) {}
  void println(long) {}
  void println(unsigned long) {}
  void println(float, int = 2) {}
  void println(double, int = 2) {}
  void println() {}
};
static PrintMock Serial __attribute__((unused));

#define F(x) (x)
