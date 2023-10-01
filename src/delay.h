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

#ifndef ASYNCINO_DELAY_H
#define ASYNCINO_DELAY_H

#include <Arduino.h>

#ifdef ASYNCINO_ID
#include "id.h"
#endif

typedef void (*ADelayCallback)();

typedef struct ADelay {
#ifdef ASYNCINO_ID
  AId id;
#endif
  unsigned long stamp;
  ADelayCallback cb;
} ADelay;

ADelay asyncino_delay_pool[ASYNCINO_DELAY_POOL];
unsigned short asyncino_delay_len = 0;

#ifdef ASYNCINO_ID
AId
#else
void
#endif
delayMicroseconds(ADelayCallback cb, unsigned long us) {
  auto stamp = micros() + us;

#ifdef ASYNCINO_ID
  AId id = aid_req();
#endif

#ifdef ASYNCINO_SAFE
  if (asyncino_delay_len < ASYNCINO_DELAY_POOL) {
#endif
    ADelay entry;
    entry.stamp = stamp;
    entry.cb = cb;
#ifdef ASYNCINO_ID
    entry.id = id;
#endif
    asyncino_delay_pool[asyncino_delay_len++] = entry;
#ifdef ASYNCINO_SAFE
  }
#endif

#ifdef ASYNCINO_ID
  return id;
#endif
}

#ifdef ASYNCINO_ID
AId
#else
void
#endif
delay(ADelayCallback cb, unsigned int ms) {
  return delayMicroseconds(cb, (long)ms * 1000);
}

void clearDelayIndex(unsigned short i) {
#ifdef ASYNCINO_SAFE
  if (i < asyncino_delay_len) {
#endif
#ifdef ASYNCINO_ID
    aid_ret(asyncino_delay_pool[i].id);
#endif
    asyncino_delay_len -= 1;
    for (int j = i; j < asyncino_delay_len; j++) {
      asyncino_delay_pool[j] = asyncino_delay_pool[j + 1];
    }
#ifdef ASYNCINO_SAFE
  }
#endif
}

#ifdef ASYNCINO_ID
bool clearDelay(AId id) {
  for (int i = 0; i < asyncino_delay_len; i++) {
    if (id == asyncino_delay_pool[i].id) {
      clearDelayIndex(i);
      return true;
    }
  }
  return false;
}
#endif

#endif
