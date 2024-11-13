#include <Arduino.h>
#include "sleep.h"

#define GPIO_EXIT_DORMANT_MODE 3 /* connect GP3 to GND once in DORMANT mode */

void setup() {
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(GPIO_EXIT_DORMANT_MODE, INPUT_PULLUP); // pull pin that will get us out of sleep mode
}

void loop() {
  if (Serial1.available() > 0) {
    (void) Serial1.read();
    digitalWrite(LED_BUILTIN, LOW);
    Serial1.end(); // disable the UART
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    // disable systick now so that no milisecond interrupts will occur
    systick_hw->csr &= ~1;
    sleep_run_from_dormant_source(DORMANT_SOURCE_XOSC);
    sleep_goto_dormant_until_pin(GPIO_EXIT_DORMANT_MODE, true, false); 
    sleep_power_up();

    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.begin(9600); // start UART again
  }
  Serial1.println("hello, world");
  delay(500);
}