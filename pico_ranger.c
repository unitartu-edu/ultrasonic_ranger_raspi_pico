#include <stdio.h>
#include "pico/stdlib.h"
#include "pico_uart_transports.h"

// The GPIO pins to which the sonar is wired and LED
#define GPIO_ECHO 6
#define GPIO_TRIGGER 7
#define LED_PIN 25

float read_range() {

  // Send an impulse trigger of 10us
  gpio_put(GPIO_TRIGGER, 1);
  sleep_us(10);
  gpio_put(GPIO_TRIGGER, 0);

  // Read how long is the echo
  uint32_t signaloff, signalon;
  do {
    signaloff = time_us_32();
  } while (gpio_get(GPIO_ECHO) == 0);

  do {
    signalon = time_us_32();
  } while (gpio_get(GPIO_ECHO) == 1);

  // Actual echo duration in us
  float dt = signalon - signaloff;

  // distance in centimeters:
  // echo duration (us) x speed of sound (m/us) / 2 (round trip) * 100
  return dt * 0.000343 / 2.0 * 100;
}

int main()
{
  // Initialise pins
  gpio_init(LED_PIN);
  gpio_init(GPIO_TRIGGER);
  gpio_init(GPIO_ECHO);

  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_set_dir(GPIO_TRIGGER, GPIO_OUT);
  gpio_set_dir(GPIO_ECHO, GPIO_IN);

  gpio_put(GPIO_TRIGGER, 0);
  sleep_us(200);

  gpio_put(LED_PIN, 1);
  sleep_ms(1000);
  gpio_put(LED_PIN, 0);
  sleep_ms(1000);

  float range;
  while (true) {

    // Get distance with ultrasonic sensor
    range = read_range();

    // LED turns on when the distance is smaller than 10 cm
    if (range > 10){
      gpio_put(LED_PIN, 0);
    } else {
      gpio_put(LED_PIN, 1);
    }

    sleep_ms(100);
  }

  return 0;
}
