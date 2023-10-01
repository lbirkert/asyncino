// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

// asyncino is the asynchronous library for the Arduino family
//
// # ASYNCINO_AID
// The
//
// # ASYNCINO_DELAY
// Enables the usage of the asynchronous delay(callback, milliseconds) and
// delayMicroseconds(callback, microseconds) functions.
//
// ## Reference
// https://www.arduino.cc/reference/en/language/functions/time/delay/
// https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
//
// ## Settings
// ASYNCINO_DELAY_POOL the maximum amount of concurrent delays. (Default: 1)
//
// ## Usage
// ```ino
// #define ASYNCINO_DELAY
// #define ASYNCINO_DELAY_POOL 1
//
// #include "asyncino.h"
// ```
//
// # ASYNCINO_PULSEIN
// Enables the usage of the asynchonous pulseIn(callback, port, value, timeout?)
// and pulseInLong(callback, port, value, timeout?) functions.
//
// ## Reference
// https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/
// https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong/
//
// ## Settings
// ASYNCINO_PULSEIN_POOL the maximum amount of concurrent pulseIns. (Default: 1)
//
// ## Usage
// ```ino
// #define ASYNCINO_PULSEIN
// #define ASYNCINO_PULSEIN_POOL 1
//
// #include "asyncino.h"
// ```

#ifndef ASYNCINO_H
#define ASYNCINO_H

#include <Arduino.h>

#ifdef ASYNCINO_DELAY
#ifndef ASYNCINO_DELAY_POOL
#define ASYNCINO_DELAY_POOL 1
#endif
#endif

#ifdef ASYNCINO_PULSEIN
#ifndef ASYNCINO_PULSEIN_POOL
#define ASYNCINO_PULSEIN_POOL 1
#endif
#endif

#ifdef ASYNCINO_DELAY
#include "delay.h";
#endif

#ifdef ASYNCINO_PULSEIN
#include "pulsein.h";
#endif

#ifdef ASYNCINO_ID
#include "id.h";
#endif

void asyncino();

void asyncino() {
  auto now = micros();

#ifdef ASYNCINO_DELAY
  for (int i = 0; i < asyncino_delay_len; i++) {
    auto entry = &asyncino_delay_pool[i];
    if (entry->stamp <= now) {
      clearDelayIndex(i);
      i -= 1;

      entry->cb();

      continue;
    }
  }
#endif

#ifdef ASYNCINO_PULSEIN
  for (int i = 0; i < asyncino_pulsein_len; i++) {
    auto entry = &asyncino_pulsein_pool[i];
    // PulseIn timeout
    if (entry->stamp != 0 && entry->stamp <= now) {
      clearPulseInIndex(i);
      i -= 1;

      entry->cb(0);

      continue;
    }

    auto current = digitalRead(entry->pin);
    if (current != entry->last) {
      entry->last = current;

      if (current != entry->value) {
        clearPulseInIndex(i);
        i -= 1;

        entry->cb(now - entry->start);

        continue;
      } else {
        entry->start = now;
      }
    }
  }
#endif
}

#endif
