# Naming
## General Naming Rules
Function names, variable names, and filenames should be descriptive; eschew abbreviation.

Types and variables should be nouns, while functions should be "command" verbs.

For example: 
```c
int num_errors; // Good.
int num_completed_connections; // Good.
```
## File Names
Filenames should start with a capital letter and have a capital letter for each new word.

Examples of acceptable file names:
```c
MyUsefulClass.cpp
MyClass.cpp
MyUsefulClass.h
MyClass.h
```
C++ files should end in .cpp and header files should end in .h.

Do not use filenames that already exist in /usr/include, such as db.h.
## Type Names
Type names start with a capital letter and have a capital letter for each new word, with no
underscores: MyClass, MyEnum.
```c
// classes and structs
class MyTable { ...
class MyClass { ...
struct MyTestClass { ...
// typedefs
typedef hash_map<UrlTableProperties *, string> MyHash;
typedef long long int DataType_AL;
// enums
enum MyEnums { ...
```
## Variable Names
### Common Variable names
```c
string table_name; // OK uses underscore.
string tablename; // OK all lowercase.
```
### Class and Struct Data Members
```c
string table_name_; // OK underscore at end.
string tablename_; // OK.
```
## Function Names
Functions should start with a lowercase letter and have a capital letter for each new word. No underscores.
```c
myFunction(MyTable& stu_name)
addTableEntry(int& stu_name)
deleteUrl(MyClass& stu_name)
```

## Reference

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)