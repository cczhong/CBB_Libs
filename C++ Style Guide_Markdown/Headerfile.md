# 1 Header Files

## 1.1 The #define guard
All header files should have #define guards to prevent multiple inclusion. The format of the
symbol name should be 
```c
__<HEADERNAME>_H__
```
To guarantee uniqueness, they should be based on the full name in a header file. For example, the file Seed.h in project VAT should have the following guard.
```c
#ifndef __SEED_H__
#define __SEED_H__

#endif // __SEED_H__
```

## 1.2 Function Parameter Ordering
When defining a function, parameter order is: inputs, then outputs.
## 1.3 Names Order of Includes
Use standard order for readability and to avoid hidden dependencies: C library, C++ library, other libraries' .h, your project's .h.
For example, 
```c
#include <sys/types.h>
#include <unistd.h>
#include <hash_map>
#include <vector>
#include "ben/project.h"
#include "zhong/czproject.h"
#include "myproject.h"
```

## Reference

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)