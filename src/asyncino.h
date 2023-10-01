// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

#pragma once

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

#ifdef ASYNCINO_LISTEN
#ifndef ASYNCINO_LISTEN_POOL
#define ASYNCINO_LISTEN_POOL 1
#endif
#endif

#ifdef ASYNCINO_DELAY
#include "delay.h";
#endif

#ifdef ASYNCINO_PULSEIN
#include "pulsein.h";
#endif

#ifdef ASYNCINO_LISTEN
#include "listen.h";
#endif

#ifdef ASYNCINO_ID
#include "id.h";
#endif

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

#ifdef ASYNCINO_LISTEN
  for (int i = 0; i < asyncino_listen_len; i++) {
    auto entry = &asyncino_listen_pool[i];
    auto current = digitalRead(entry->pin);
    if(listenShouldTrigger(entry->type, entry->last, current)) {
      entry->cb(current);
    }
    entry->last = current;
  }
#endif

}

#endif
