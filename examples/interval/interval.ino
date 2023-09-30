// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

// This programm asynchronously prints "Hello World" every second

// enable asyncino's delay feature
#define ASYNCINO_DELAY
#define ASYNCINO_DELAY_POOL 1
// enable safety assertions to prevent seg faults
#define ASYNCINO_SAFE

#include <asyncino.h>

void helloworld() {
  Serial.println("Hello World!");

  delay(helloworld, 1000);
}

void setup() {
  Serial.begin(9600);

  helloworld();
}

void loop() {
  // run asyncino's eventloop
  asyncino();
}
