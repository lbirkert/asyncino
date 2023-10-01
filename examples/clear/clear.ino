// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

// This programm asynchronously blinks and stops when STOP_PIN is pulled LOW.

// enable asyncino's delay, pulseIn and id feature
#define ASYNCINO_DELAY
#define ASYNCINO_PULSEIN
#define ASYNCINO_ID
// enable safety assertions to prevent seg faults
#define ASYNCINO_SAFE

#include <asyncino.h>

#define STOP_PIN 7

bool state = LOW;
AId intv = 0;

// blink asynchronously
void blink() {
  state = !state;
  digitalWrite(LED_BUILTIN, state);
  intv = delay(blink, 100);
}

// listen when pin 7 drops to LOW and cancel blinking
void listenStop() {
  pulseIn([]() {
    clearDelay(intv);
  }, STOP_PIN, HIGH);
}

void setup() {
  pinMode(STOP_PIN, INPUT_PULLUP);

  blink();
  listenStop();
}

void loop() {
  //Serial.println("STATE: " + String(digitalRead(STOP_PIN)));
  // run asyncino's eventloop
  asyncino();
}
