# Other Freatures
### Prefer consts, enums, and inlines to #define
If you define the aspect_ratio as follows:
```c
#define aspect_ratio 1.653 
```
We recommend that you use the following code to replace it:
```c
const double aspect_ratio = 1.653
```
When you need a constant value during the compilation phase, const is powerless. At this time you need enum.

For example, if you define an array:
```c
int scores[numTurns];
```
The compiler insists on knowing the size of numTurns. You can use enum at this time:
```c
enum {numTurns = 5};
```
Using #define to implement a macro, the function is the same as a function, but there is no overhead when the function is called, but the macro definition check causes unnecessary errors. The classic one is the comparison of two numbers:
```c
#define CALL_WITH_MAX(a,b)  f((a)>(b)?(a):(b))
```
Even if you add parentheses to each variable, there is still an error, that is, when you pass in ++ a, b, when a> b, a plus two times will happen, and you all understand it. You can use the following statement instead:
```c
template <typename T>
inline void callWithMax(const T& a,const T& b)
{
    f(a>b ? a : b);
}
```
### Use const whatever possible
If you define the myVariable as follows:
```c
int myVariable = 0;
myVariable = ComputeFactor(params...);
```
We recommend that you use the following code to replace it:
```c
const int myVariable = ComputeFactor(params...);
```
If you define the str as follows:
```c
string str;
str = "Hello World";
```
We recommend that you use the following code to replace it:

```c
const string str = "Hello World";
```

### Casting
Use C++ casts like static_cast<>(). Do not use other cast formats like int y =
(int)x; or int y = int(x);.
The problem with C casts is the ambiguity of the operation; sometimes you are doing a conversion
and sometimes you are doing a cast (e.g., (int)"hello"); C++ casts avoid
this. Additionally C++ casts are more visible when searching for them.
### Preincrement and Predecrement
Use prefix form (++i) of the increment and decrement operators with iterators and other template objects.
When the return value is ignored, the "pre" form (++i) is never less efficient than the "post" form
(i++), and is often more efficient. This is because postincrement
(or decrement) requires a copy of
i to be made, which is the value of the expression. If i is an iterator or other nonscalar
type, copying i could be expensive. Since the two types of increment behave the same when the value
is ignored, why not just always preincrement?

### Integer Types
Of the builtin
C++ integer types, the only one used is int. If a program needs a variable of a
different size, use a precisewidth
integer type from <stdint.h>, such as int16_t.

The sizes of integral types in C++ can vary based on compiler and architecture.

### 0 and NULL
Use 0 for integers, 0.0 for reals, NULL for pointers, and '\0' for chars.

For pointers (address values), there is a choice between 0 and NULL. Bjarne Stroustrup prefers an
unadorned 0. We prefer NULL because it looks like a pointer. In fact, some C++ compilers, such as
gcc 4.1.0, provide special definitions of NULL which enable them to give useful warnings,
particularly in situations where sizeof(NULL) is not equal to sizeof(0).
Use '\0' for chars. This is the correct type and also makes code more readable.

### C++11

Use only approved libraries and language extensions from C++11 (formerly known as C++0x). Currently, none are approved. 

C++11 has become the official standard, and eventually will be supported by most C++ compilers. It standardizes some common C++ extensions that we use already, allows shorthands for some operations, and has some performance and safety improvements.

## Reference

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)