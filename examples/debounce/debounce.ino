// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

// This programm counts the amount a button was clicked and Serial.prints the results.

// Enable asyncino's delay, listen and id feature
#define ASYNCINO_DELAY
#define ASYNCINO_LISTEN
#define ASYNCINO_ID
#define ASYNCINO_SAFE // disable undefined behavior

#include <asyncino.h>

#define BTN 7
#define BTN_DEBOUNCE 10 // 10ms

unsigned int counter = 0;

bool state = HIGH;
AId delayId = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BTN, INPUT_PULLUP);

  listen([](int value) {
    if(delayId != 0) {
      clearDelay(delayId);
    } else {
      delayId = delay([](){
        if(state) {
          counter += 1;
          Serial.println("CLICKED: " + String(counter));
        }

        state = !state;
      }, BTN_DEBOUNCE);
    }
  }, BTN, ALISTEN_CHANGE);
}

void loop() {
  // run asyncino's eventloop
  asyncino();
}
