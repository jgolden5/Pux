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

#define SPEAKER_PIN 8
#define LED_PIN 16

const uint8_t buttonPins[] = { 12, 11, 10, 9, 7, 6, 5, 4 };
const int buttonTones[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5 
};
const int numTones = sizeof(buttonPins) / sizeof(buttonPins[0]);

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

int main() {
  stdio_init_all();
  for(uint8_t i = 0; i < numTones; i++) {
    gpio_init(buttonPins[i]);
    gpio_set_dir(buttonPins[i], GPIO_IN);
    gpio_pull_up(buttonPins[i]);
  }
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
  while(true) {
    int pitch = 0;
    for(uint8_t i = 0; i < numTones; i++) {
      if(!gpio_get(buttonPins[i])) {
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
