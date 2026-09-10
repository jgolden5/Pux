#include "mux.h"

Mux::Mux() {
  for(int i = 0; i < 4; i++) {
    states[i] = false;
  }
}
void Mux::on(int pin) {
  if(pin >= 0 && pin < 4) {
    states[pin] = true;
  }
}
void Mux::off(int pin) {
  if(pin >= 0 && pin < 4) {
    states[pin] = false;
  }
}
int Mux::getValue() {
  return
    (states[0] << 0) |
    (states[1] << 1) |
    (states[2] << 2) |
    (states[3] << 3);
}
