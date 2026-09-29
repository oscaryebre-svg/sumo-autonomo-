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

inline void pinMode(uint8_t, uint8_t) {}
inline void digitalWrite(uint8_t, uint8_t) {}
inline int digitalRead(uint8_t) { return 0; }
inline void analogWrite(uint8_t, int) {}
inline int analogRead(uint8_t) { return 0; }
inline unsigned long millis() { return 0; }
inline void delay(unsigned long) {}

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
