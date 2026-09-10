/**
   Mini piano for the Raspberry Pi Pico.

   You can control the colorful buttons with your keyboard:
   After starting the simulation, click anywhere in the diagram to focus it.
   Then press any key between 1 and 8 to play the piano (1 is the lowest note,
   8 is the highest).

   Copyright (C) 2021, Uri Shaked. Released under the MIT License.
*/

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pitches.h"
#include "mux.h"

#define SPEAKER_PIN 8
#define MUX_PIN 16
#define LED_PIN 20

const uint8_t pianoButtonPins[] = { 12, 11, 10, 9, 7, 6, 5, 4 };
const int buttonTones[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5 
};
const int numTones = sizeof(pianoButtonPins) / sizeof(pianoButtonPins[0]);

void playTone(uint gpio, uint frequency) {
  uint slice = pwm_gpio_to_slice_num(gpio);
  uint channel = pwm_gpio_to_channel(gpio);
  gpio_set_function(gpio, GPIO_FUNC_PWM);
  float divider = 8.0f;
  pwm_set_clkdiv(slice, divider);
  const uint32_t pwmClock = 125000000 / divider;
  uint32_t wrap = pwmClock / frequency - 1;
  if (wrap > 65535) {
    wrap = 65535;
  }
  pwm_set_wrap(slice, wrap);
  pwm_set_chan_level(slice, channel, wrap / 2);
  pwm_set_enabled(slice, true);
}

void stopTone(uint gpio) {
  uint slice = pwm_gpio_to_slice_num(gpio);
  pwm_set_enabled(slice, false);
}

void initialize_piano_buttons() {
  for(uint8_t i = 0; i < numTones; i++) {
    gpio_init(pianoButtonPins[i]);
    gpio_set_dir(pianoButtonPins[i], GPIO_IN);
    gpio_pull_up(pianoButtonPins[i]);
  }
  gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
}

void initialize_mux_buttons() {
  gpio_init(MUX_PIN);
  gpio_set_dir(MUX_PIN, GPIO_IN);
  gpio_pull_up(MUX_PIN);
}

void initialize_leds() {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0);
}

int main() {
  stdio_init_all();
  initialize_piano_buttons();
  initialize_mux_buttons();
  Mux mux = Mux();
  initialize_leds();
  while(true) {
    bool pressed = !gpio_get(MUX_PIN);
    if(pressed) {
      gpio_put(LED_PIN, 1);
    } else {
      gpio_put(LED_PIN, 0);
    }
    int pitch = 0;
    for(uint8_t i = 0; i < numTones; i++) {
      if(!gpio_get(pianoButtonPins[i])) {
        pitch = buttonTones[i];
      }
    }
    if(pitch != 0) {
      playTone(SPEAKER_PIN, pitch);
    } else {
      stopTone(SPEAKER_PIN);
    }
    sleep_ms(10);
  }
  return 0;
}
