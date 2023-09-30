// SPDX-License-Identifier: MIT

//                                _
//   ____ ________  ______  _____(_)___  ____
//  / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
// / /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
// \__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
//           /____/
//
// (c) Copyright 2023 Lucas Birkert, all rights reserved

// This programm asynchronously gets the distance from an ultrasonic sensor using asyncino

// enable asyncino's delay feature
#define ASYNCINO_DELAY
#define ASYNCINO_DELAY_POOL 1
// enable asyncino's pulsein feature
#define ASYNCINO_PULSEIN
#define ASYNCINO_PULSEIN_POOL 1
// enable safety assertions to prevent seg faults
#define ASYNCINO_SAFE

#include <asyncino.h>

// define the ultrasonic trigger & echo pins
#define USS_TRIG 7
#define USS_ECHO 8

// get the current distance from the ultrasonic sensor
void scan() {
  // emit sound for 10ms
  digitalWrite(USS_TRIG, HIGH);
  delay([]() {
    digitalWrite(USS_TRIG, LOW);

    // wait until echo received
    pulseIn([](unsigned long duration) {
      if(duration != 0) {
        Serial.print("DISTANCE: ");
        Serial.println(duration * 0.017156); // convert duration to distance
      }

      // schedule next scan
      delay(scan, 100);
    }, USS_ECHO, HIGH, 10000);
  }, 10);
}

void setup() {
  Serial.begin(9600);

  pinMode(USS_TRIG, OUTPUT);
  pinMode(USS_ECHO, INPUT);

  scan();
}

void loop() {
  // run asyncino's eventloop
  asyncino();
}
