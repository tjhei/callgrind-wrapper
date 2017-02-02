# callgrind-wrapper

This is a simple c++ wrapper around callgrind to query current instruction
counter values.

## Usage

1. ``#include "callgrind-wrapper.h"``
2. instrument a block of code using
    ```
    {
      callgrind_wrapper::Instrument block("transmutate");
      transmutate();
    }
    ```
    or
    ```
    callgrind_wrapper::reset();
    transmutate();
    long cycles = callgrind_wrapper::current();
    ```
    
3. run using 
```
  valgrind --tool=callgrind -q --combine-dumps=yes --callgrind-out-file=callgrind.out ./executable
```
and you will get the following output:
```
> transmutate 123534
```
