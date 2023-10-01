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

#ifndef ASYNCINO_LISTEN_H
#define ASYNCINO_LISTEN_H

#include <Arduino.h>

#ifdef ASYNCINO_ID
#include "id.h"
#endif

#define ALISTEN_LOW 0
#define ALISTEN_HIGH 1
#define ALISTEN_FALLING 2
#define ALISTEN_RISING 3
#define ALISTEN_CHANGE 4

typedef void (*AListenCallback)(int current);

struct AListen {
#ifdef ASYNCINO_ID
  AId id;
#endif
  AListenCallback cb;
  int pin;
  int type;
  int last;
};

AListen asyncino_listen_pool[ASYNCINO_LISTEN_POOL];
unsigned short asyncino_listen_len = 0;

#ifdef ASYNCINO_ID
AId
#else
void
#endif
listen(AListenCallback cb, int pin, int type) {
#ifdef ASYNCINO_ID
  auto id = aid_req();
#endif

#ifdef ASYNCINO_SAFE
  if (asyncino_listen_len < ASYNCINO_LISTEN_POOL) {
#endif

    AListen entry;
    entry.cb = cb;
    entry.pin = pin;
    entry.type = type;
    entry.last = digitalRead(pin);
#ifdef ASYNCINO_ID
    entry.id = id;
#endif
    asyncino_listen_pool[asyncino_listen_len++] = entry;

#ifdef ASYNCINO_SAFE
  }
#endif

#ifdef ASYNCINO_ID
  return id;
#endif
}

void clearListenIndex(unsigned int i) {
#ifdef ASYNCINO_SAFE
  if (i < asyncino_listen_len) {
#endif
#ifdef ASYNCINO_ID
    aid_ret(asyncino_listen_pool[i].id);
#endif
    asyncino_listen_len -= 1;
    for (int j = i; j < asyncino_listen_len; j++) {
      asyncino_listen_pool[j] = asyncino_listen_pool[j + 1];
    }
#ifdef ASYNCINO_SAFE
  }
#endif
}

#ifdef ASYNCINO_ID
bool clearListen(AId id) {
  for (int i = 0; i < asyncino_listen_len; i++) {
    if (id == asyncino_listen_pool[i].id) {
      clearListenIndex(i);
      return true;
    }
  }
  return false;
}
#endif

bool listenShouldTrigger(int type, int last, int current) {
  switch(type) {
    case ALISTEN_LOW: case ALISTEN_HIGH:
      return current == type;
    case ALISTEN_FALLING:
      return last == HIGH && current == LOW;
    case ALISTEN_RISING:
      return last == LOW && current == HIGH;
    case ALISTEN_CHANGE:
      return last != current;
    default:
      return false;
  }
}

#endif
