[![.github/workflows/lint.yaml](https://github.com/lbirkert/asyncino/actions/workflows/lint.yaml/badge.svg)](https://github.com/lbirkert/asyncino/actions/workflows/lint.yaml)

```
                               _
  ____ ________  ______  _____(_)___  ____
 / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
/ /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
\__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
          /____/
```

An asynchronous library for the Arduino family. [Examples](https://github.com/lbirkert/asyncino/tree/main/examples)

----

<br/>

## Feature ASYNCINO_DELAY

Enables the usage of the asynchronous `delay(callback, milliseconds)` and
`delayMicroseconds(callback, microseconds)` functions.

### Arduino Reference

https://www.arduino.cc/reference/en/language/functions/time/delay/ <br/>
https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/

### Settings

`ASYNCINO_DELAY_POOL`: the maximum amount of concurrent delays. (default: `1`)

### Usage

```ino
#define ASYNCINO_DELAY

#include "asyncino.h"

void setup() {
    Serial.println("Hello World");

    delay([]() {
        Serial.println("1s later: Hello World");
        delay([]() {
            Serial.println("1.5s later: Hello World");
        }, 500);
    }, 1000);
    
    delay([]() {
        Serial.println("0.5s later: Hello World");
    }, 500);
}

void loop() {
    asyncino();
}
```

----

<br/>

## Feature ASYNCINO_PULSEIN

Enables the usage of the asynchonous `pulseIn(callback, port, value, timeout?)` and
`pulseInLong(callback, port, value, timeout?)` functions.

### Arduino Reference

https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/ <br/>
https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong/

### Settings

`ASYNCINO_PULSEIN_POOL`: the maximum amount of concurrent pulseIns. (default: `1`)

### Usage

```ino
#define ASYNCINO_PULSEIN

#include "asyncino.h"

void setup() {
    pinMode(7, INPUT_PULLUP);
    // waits for pulse (HIGH => LOW: start timer, LOW => HIGH: stop timer)
    pulseIn([](unsigned long duration) {
        Serial.println("pin 7 received a pulse: " + String(duration) + "us");
    }, 7, LOW);
}

void loop() {
    asyncino();
}
```

----

<br/>

## Feature ASYNCINO_ID

Enables the usage of ids, which can be used to clear delays and pulseIns. (`clearDelay(id)`, `clearPulseIn(id)`)

### Settings

`ASYNCINO_ID_POOL`: the size of the free id ring buffer. (default: `ASYNCINO_PULSEIN_POOL + ASYNCINO_DELAY_POOL`)

### Usage

```ino
#define ASYNCINO_ID
#define ASYNCINO_DELAY

#include "asyncino.h"

void setup() {
    const AId myDelay = delay([]() {
        Serial.println("MyDelay was executed"); // This should not be printed
    }, 2000);

    delay([]() {
        Serial.println("MyDelay was cleared");
        clearDelay(myDelay); // Cancels a pending delay
    }, 1000);
}

void loop() {
    asyncino();
}
```

----

<br/>

## Feature ASYNCINO_LISTEN

Enables the usage of `listen(cb, port, type)`, which can be used to listen for certain changes on a pin.

type can be one of the following:

|value | constant | description|
|--|--|--|
|0|`ALISTEN_LOW`| run cb whenever pin is LOW|
|1|`ALISTEN_HIGH`| run cb whenever pin is HIGH|
|2|`ALISTEN_FALLING`| run cb whenever pin goes from HIGH to LOW|
|3|`ALISTEN_RISING`| run cb whenever pin goes from LOW to HIGH|
|4|`ALISTEN_CHANGE`| run cb whenever pin value changes (RISING or FALLING)|

### Settings

`ASYNCINO_LISTEN_POOL`: the maximum amount of concurrent listens. (default: `1`)

### Usage

```ino
#define ASYNCINO_LISTEN

#include "asyncino.h"

void setup() {
    pinMode(7, INPUT_PULLUP);

    // watch pin 7 for changes
    listen([](int value) {
        Serial.println("PIN CHANGED: " + String(value));
    }, 7, ALISTEN_CHANGE); 
}

void loop() {
    asyncino();
}
```

----

<br/>

&copy; Copyright 2023 [Lucas Birkert](https://lbirkert.com), all rights reserved
