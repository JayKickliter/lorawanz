## What is this?

This is a simple LoRaWAN application meant to run on small embedded
microcontrollers on top of the Zephyr real time operating system
(RTOS). This application is based directly on Zephyr's [sample]
LoRaWAN application, but has been extracted into this repo for both
better visibility and to serve as an example for:

- out-of-tree Zephyr application
- building and debugging a Zephyr application without [west]
- (coming soon) custom/out-of-tree boards

[sample]: https://github.com/zephyrproject-rtos/zephyr/tree/master/samples/lorawan/class_a
[west]: https://docs.zephyrproject.org/latest/guides/west/index.html

## Dependencies

You need to install the Zephyr source and its dependencies before you
can build this application. Installing Zephyr is way out of scope of
this README, but at least they have it very well [documented].

[documented]: https://docs.zephyrproject.org/latest/getting_started/index.html

## Device personality

By default, this application uses zeros for DevEUI, JoinEUI, and
AppKey. In order to talk to your LoRaWAN network server, create a file
in `src/` named `personality.c` with the following structure:

```c
#include "personality.h"

uint8_t LORAWAN_DEV_EUI[8]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
uint8_t LORAWAN_JOIN_EUI[8] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
uint8_t LORAWAN_APP_KEY[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
```

Remember that the hex values are above are just examples, and that you
need to use actual values. If you are generating these on Helium
console, expand them and copy the MSB representation for each.

## Known compatible eval boards

Zephyr's driver model makes applications extremely portable, so this
list is not comprehensive. 

| Board               | Command line identifier |
|---------------------|-------------------------|
| [B-L072Z-LRWAN1]    | `b_l072z_lrwan1`        |
| [96 Boards WisTrio] | `96b_wistrio`           |

[B-L072Z-LRWAN1]: https://www.digikey.com/en/products/detail/stmicroelectronics/B-L072Z-LRWAN1/6616000
[96 Boards WisTrio]: https://www.96boards.org/product/wistrio

## Building

### With west (easiest)

```
$ west build --board b_l072z_lrwan1
```

### With CMake and ninja

```
$ mkdir build
$ cd build
$ cmake -GNinja -DBOARD=b_l072z_lrwan1 ..
$ ninja
```

### With CMake and make

```
$ mkdir build
$ cd build
$ cmake -DBOARD=b_l072z_lrwan1 ..
$ make
```

## Debugging

In one shell, start GDB server:

```
$ JLinkGDBServer -select usb -port 2331 -if swd -speed 4000 -device STM32L072CZ -silent
```

(optional) Start an RTT console in second shell to view log messages:

```
$ JLinkRTTClient
```

Start GDB in a third shell:

```
$ cgdb -d arm-none-eabi-gdb build/zephyr/zephyr.elf
(gdb) target extended-remote :2331
(gdb) start
(gdb) continue
```
