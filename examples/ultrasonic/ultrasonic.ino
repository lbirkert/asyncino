// enables asyncino's delay feature
#define ASYNCINO_DELAY
#define ASYNCINO_DELAY_POOL 1
// enables asyncino's pulsein feature
#define ASYNCINO_PULSEIN
#define ASYNCINO_PULSEIN_POOL 1
// assures safety assertions to prevent seg faults
#define ASYNCINO_SAFE

#include "asyncino.h"

#define USS_TRIG 7
#define USS_ECHO 8

void scan() {
  digitalWrite(USS_TRIG, HIGH);
  
  delay([]() {
    digitalWrite(USS_TRIG, LOW);

    pulseIn([](unsigned long duration) {
      Serial.println("DURATION: " + String(duration));
    }, USS_ECHO, HIGH, 100000);

    delay(scan, 100);
  }, 10);
}

void setup() {
  Serial.begin(9600);

  pinMode(USS_TRIG, OUTPUT);
  pinMode(USS_ECHO, INPUT);

  scan();
}

void loop() {
  asyncino();
}
