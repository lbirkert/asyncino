```
                               _
  ____ ________  ______  _____(_)___  ____
 / __ `/ ___/ / / / __ \/ ___/ / __ \/ __ \
/ /_/ (__  ) /_/ / / / / /__/ / / / / /_/ /
\__,_/____/\__, /_/ /_/\___/_/_/ /_/\____/
          /____/

(c) Copyright 2023 Lucas Birkert, all rights reserved
```

asyncino is the asynchronous library for the Arduino family. [Examples](https://github.com/lbirkert/asyncino/tree/main/examples)

----

<br/>

## Feature ASYNCINO_DELAY

Enables the usage of the asynchronous delay(callback, milliseconds) and
delayMicroseconds(callback, microseconds) functions.

### Reference

https://www.arduino.cc/reference/en/language/functions/time/delay/ <br/>
https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/

### Settings

ASYNCINO_DELAY_POOL: the maximum amount of concurrent delays.

### Usage

```ino
#define ASYNCINO_DELAY
#define ASYNCINO_DELAY_POOL 1

#include "asyncino.h"
```

----

<br/>

## Feature ASYNCINO_PULSEIN

Enables the usage of the asynchonous pulseIn(callback, port, value, timeout?) and
pulseInLong(callback, port, value, timeout?) functions.

### Reference

https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/ <br/>
https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong/

### Settings

ASYNCINO_PULSEIN_POOL: the maximum amount of concurrent pulseIns.

### Usage

```ino
#define ASYNCINO_PULSEIN
#define ASYNCINO_PULSEIN_POOL 1

#include "asyncino.h"
```
