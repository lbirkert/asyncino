#pragma once

#ifndef ASYNCINO_PULSEIN_H
#define ASYNCINO_PULSEIN_H

#include <Arduino.h>

#ifndef ASYNCINO_PULSEIN_POOL
#define ASYNCINO_PULSEIN_POOL 1
#endif

#ifdef ASYNCINO_ID
#include "id.h"
#endif

typedef void (*PulseInCallback)(unsigned long);

struct APulseIn {
#ifdef ASYNCINO_ID
  AId id;
#endif
  unsigned long stamp;
  unsigned long start;
  PulseInCallback cb;
  int pin;
  int value;
  int last;
};

#ifdef ASYNCINO_ID
AId
#else
void
#endif
pulseIn(PulseInCallback cb, int pin, int value, unsigned long timeout = 0);

#ifdef ASYNCINO_ID
AId
#else
void
#endif
pulseInLong(PulseInCallback cb, int pin, int value, unsigned long timeout = 0);

void clearPulseInIndex(unsigned int i);

#ifdef ASYNCINO_ID
bool clearPulseIn(AId id);
#endif

APulseIn asyncino_pulsein_pool[ASYNCINO_PULSEIN_POOL];
unsigned short asyncino_pulsein_len = 0;

#ifdef ASYNCINO_ID
AId
#else
void
#endif
pulseIn(PulseInCallback cb, int pin, int value, unsigned long timeout) {
  auto now = micros();
  auto stamp = timeout == 0 ? 0 : now + timeout;

#ifdef ASYNCINO_ID
  auto id = aid_req();
#endif

#ifdef ASYNCINO_SAFE
  if (asyncino_pulsein_len < ASYNCINO_PULSEIN_POOL) {
#endif

    APulseIn entry;
    entry.stamp = stamp;
    entry.start = now;
    entry.cb = cb;
    entry.pin = pin;
    entry.value = value;
    entry.last = digitalRead(pin);
#ifdef ASYNCINO_ID
    entry.id = id;
#endif
    asyncino_pulsein_pool[asyncino_pulsein_len++] = entry;

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
pulseInLong(PulseInCallback cb, int pin, int value,
                 unsigned long timeout) {
  return pulseIn(cb, pin, value, timeout);
}

#ifdef ASYNCINO_ID
bool clearPulseIn(AId id) {
  for (int i = 0; i < asyncino_pulsein_len; i++) {
    if (id == asyncino_pulsein_pool[i].id) {
      clearPulseInIndex(i);
      return true;
    }
  }
  return false;
}
#endif

void clearPulseInIndex(unsigned int i) {
#ifdef ASYNCINO_SAFE
  if (i < asyncino_pulsein_len) {
#endif
#ifdef ASYNCINO_ID
    aid_ret(asyncino_pulsein_pool[i].id);
#endif
    asyncino_pulsein_len -= 1;
    for (int j = i; j < asyncino_pulsein_len; j++) {
      asyncino_pulsein_pool[j] = asyncino_pulsein_pool[j + 1];
    }
#ifdef ASYNCINO_SAFE
  }
#endif
}

#endif
