# Scoping

## Namespaces
Unnamed namespaces in .cpp files are encouraged. With named namespaces, choose the
name based on the project, and possibly its path. 
### Unnamed Namespaces
Unnamed namespaces are allowed and even encouraged in .cpp files, to avoid runtime
naming conflicts:
```c
myspace 
{ 
    // This is in TestFile.cpp file.
    // The content of a namespace is not indented
    enum { kUnused, kEOF, kError }; // Commonly used tokens.
    bool atEof() { return pos_ == kEOF; } // Uses our namespace's EOF.
} // myspace

```
Do not use unnamed namespaces in .h files.

### Named namespaces should be used as follows:
Namespaces wrap the entire source file after includes, gflags definitions/declarations, and
forward declarations of classes from other namespaces:

```c
// In the .h file
namespace mynamespace 
{
// All declarations are within the namespace scope.
// Notice the lack of indentation.
    class MyClass 
    {
    public:
    ...
        void addTest();
    };
} // namespace mynamespace

```
```c
// In the .cpp file
namespace mynamespace {
// Definition of functions is within scope of the namespace.
void MyClass::addTest() 
{
...
}
} // namespace mynamespace
```

Do not declare anything in namespace std, not even forward declarations of standard library
classes. Declaring entities in namespace std is undefined behavior, i.e., not portable. 

To declare entities from the standard library, include the appropriate header file.
You may not use a usingdirective to make all names from a namespace available.
```c
// Forbidden This pollutes the namespace.
using namespace foo;
```

## Reference

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)