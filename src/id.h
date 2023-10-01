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

#ifndef ASYNCINO_PULSEIN_POOL
#define ASYNCINO_PULSEIN_POOL 0
#endif
#ifndef ASYNCINO_DELAY_POOL
#define ASYNCINO_DELAY_POOL 0
#endif

#ifndef ASYNCINO_ID_H
#define ASYNCINO_ID_H

#ifndef ASYNCINO_ID_POOL
#define ASYNCINO_ID_POOL ASYNCINO_DELAY_POOL + ASYNCINO_PULSEIN_POOL
#endif

typedef unsigned short AId;
AId aid_req();
void aid_ret(AId id);

AId asyncino_id_pool[ASYNCINO_ID_POOL];
unsigned short asyncino_id_write = 0;
unsigned short asyncino_id_read = 0;

AId asyncino_id_counter = 0;

AId aid_req() {
  if (asyncino_id_read != asyncino_id_write) {
    return asyncino_id_pool[asyncino_id_read++];
    asyncino_id_read %= ASYNCINO_ID_POOL;
  } else {
    return asyncino_id_counter++;
  }
}

void aid_ret(AId id) {
  unsigned short tmp_write = asyncino_id_write + 1;
  tmp_write %= ASYNCINO_ID_POOL;
  if (asyncino_id_read != tmp_write) {
    asyncino_id_pool[asyncino_id_write] = id;
    asyncino_id_write = tmp_write;
  }
}

#endif
