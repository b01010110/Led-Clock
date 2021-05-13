#include <GyverTimers.h>
#include "clock.h"

Clock clock(6, 7, 9, 11, 10, 8, 4, 5, 1, 0, 2, 3, 13, 12);

void setup() {
  clock.init();

  Timer2.setPeriod(5000);
  Timer2.enableISR(CHANNEL_A);
}

ISR(TIMER2_A) {
  clock.displayData();
}

void loop() {
  clock.tick();
}
