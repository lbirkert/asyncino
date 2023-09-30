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
// # ASYNCINO_DELAY
// Enables the usage of the asynchronous delay(callback, milliseconds) and
// delayMicroseconds(callback, microseconds) functions.
//
// ## Reference
// https://www.arduino.cc/reference/en/language/functions/time/delay/
// https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
//
// ## Settings
// ASYNCINO_DELAY_POOL the maximum amount of concurrent delays.
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
// Enables the usage of the asynchonous pulseIn(callback, port, value, timeout?) and
// pulseInLong(callback, port, value, timeout?) functions.
//
// ## Reference
// https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/
// https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong/
//
// ## Settings
// ASYNCINO_PULSEIN_POOL the maximum amount of concurrent pulseIns.
//
// ## Usage
// ```ino
// #define ASYNCINO_PULSEIN
// #define ASYNCINO_PULSEIN_POOL 1
//
// #include "asyncino.h"
// ```

#ifdef ASYNCINO_DELAY 
  typedef void (*DelayCallback)();
#endif

#ifdef ASYNCINO_PULSEIN
  typedef void (*PulseInCallback)(unsigned long);
#endif

#ifdef ASYNCINO_DELAY
  struct Delay {
    unsigned long stamp;
    DelayCallback cb;
  } asyncino_delay_pool[ASYNCINO_DELAY_POOL];
  unsigned short asyncino_delay_len = 0;

  void delayMicroseconds(DelayCallback cb, unsigned long us) {
    auto stamp = micros() + us;

    #ifdef ASYNCINO_SAFE
      if(asyncino_delay_len < ASYNCINO_DELAY_POOL) {
    #endif

    Delay entry;
    entry.stamp = stamp;
    entry.cb = cb;

    asyncino_delay_pool[asyncino_delay_len++] = entry;

    #ifdef ASYNCINO_SAFE
      }
    #endif
  }

  void delay(DelayCallback cb, unsigned int ms) {
    delayMicroseconds(cb, (long) ms * 1000);
  }
#endif


#ifdef ASYNCINO_PULSEIN
  struct PulseIn {
    unsigned long stamp;
    unsigned long start;
    PulseInCallback cb;
    int pin;
    int value;
    int last;
  } asyncino_pulsein_pool[ASYNCINO_PULSEIN_POOL];
  unsigned short asyncino_pulsein_len = 0;
  void pulseIn(PulseInCallback cb, int pin, int value, unsigned long timeout = 0) {
    auto now = micros();
    auto stamp = timeout == 0 ? 0 : now + timeout;

    #ifdef ASYNCINO_SAFE
      if(asyncino_pulsein_len < ASYNCINO_PULSEIN_POOL) {
    #endif

    PulseIn entry;
    entry.stamp = stamp;
    entry.start = now;
    entry.cb = cb;
    entry.pin = pin;
    entry.value = value;
    entry.last = digitalRead(pin);

    asyncino_pulsein_pool[asyncino_pulsein_len++] = entry;

    #ifdef ASYNCINO_SAFE
      }
    #endif
  }
  
  void pulseInLong(PulseInCallback cb, int pin, int value, unsigned long timeout = 0) {
    pulseIn(cb, pin, value, timeout);
  }
#endif

void asyncino() {
  auto now = micros();

  #ifdef ASYNCINO_DELAY
    for(int i = 0; i < asyncino_delay_len; i++) {
      auto entry = &asyncino_delay_pool[i];
      if(entry->stamp <= now) {
        asyncino_delay_len -= 1;
        i -= 1;

        for(int j = i; j < asyncino_delay_len; j++) {
          asyncino_delay_pool[j] = asyncino_delay_pool[j + 1];
        }

        entry->cb();

        continue;
      }
    }
  #endif

  #ifdef ASYNCINO_PULSEIN
    for(int i = 0; i < asyncino_pulsein_len; i++) {
      auto entry = &asyncino_pulsein_pool[i];
      // PulseIn timeout
      if(entry->stamp != 0 && entry->stamp <= now) {
        asyncino_pulsein_len -= 1;
        i -= 1;

        for(int j = i; j < asyncino_pulsein_len; j++) {
          asyncino_pulsein_pool[j] = asyncino_pulsein_pool[j + 1];
        }

        entry->cb(0);

        continue;
      }

      auto current = digitalRead(entry->pin);
      if(current != entry->last) {
        entry->last = current;

        if(current != entry->value) {
          asyncino_pulsein_len -= 1;
          i -= 1;

          for(int j = i; j < asyncino_pulsein_len; j++) {
            asyncino_pulsein_pool[j] = asyncino_pulsein_pool[j + 1];
          }

          entry->cb(now - entry->start);

          continue;
        } else {
          entry->start = now;
        }
      }

    }
  #endif
}
