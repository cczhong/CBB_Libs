# Class

If a class has ten or more public methods, excluding constructor, destructor, access methods, and private/protected methods, we should split the class into smaller classes.

For example, in the Graph class, methods, like getNode(), getSize(), isEmpty() etc. should not be counted as those 10 public methods; while DFS(), removeNode() etc. should be counted. 

For the Graph class, we only have two public methods, DFS() and removeNode(), although there are more than ten methods. 
```c
class Graph 
{
   public:
   Graph(){};
   Graph(..,..){};
   Graph(..,..,..){};
   Graph(..,..,..,..){};
   DFS();
   removeNode();
   private:
   int getNode();
   int getSize();
   bool isEmpty();
   int max_vertex_
   int num_vertex_
   int num_edge_;
   char* vertex;
   int** edge;
};
```
# Nested Classes
A class can define another class within it; this is also called a member class.

Normally, the number of nested classes cannot exceed two.
```c
class Test 
{
private:
// MyTest is a member class, nested within Test.
    class MyTest 
    {
    ...
    };
};
```
# Interface
A class is made abstract by declaring at least one of its functions as pure virtual function. A pure virtual function is specified by placing "= 0" in its declaration as follows:
```c
class Box {
   public:
      // pure virtual function
      virtual double getVolume() = 0;
      
   private:
      double length;      // Length of a box
      double breadth;     // Breadth of a box
      double height;      // Height of a box
};
```

## Abstract Class Example
Consider the following example where the parent class provides an interface to the base class to implement a function called getArea() −
```c
#include <iostream>
 
using namespace std;
 
// Base class
class Shape {
   public:
      // pure virtual function providing interface framework.
      virtual int getArea() = 0;
      void setWidth(int w) {
         width = w;
      }
   
      void setHeight(int h) {
         height = h;
      }
   
   protected:
      int width;
      int height;
};
 
// Derived classes
class Rectangle: public Shape {
   public:
      int getArea() { 
         return (width * height); 
      }
};

class Triangle: public Shape {
   public:
      int getArea() { 
         return (width * height)/2; 
      }
};
 
int main(void) 
{
   Rectangle Rect;
   Triangle  Tri;
 
   Rect.setWidth(5);
   Rect.setHeight(7);
   
   // Print the area of the object.
   cout << "Total Rectangle area: " << Rect.getArea() << endl;

   Tri.setWidth(5);
   Tri.setHeight(7);
   
   // Print the area of the object.
   cout << "Total Triangle area: " << Tri.getArea() << endl; 

   return 0;
}
```

# Other Design
![avatar](./1.png)
```c
class Essential {
   public:
     double getVolume() = 0;
      
   private:
      double length;      // Length of a box
      double breadth;     // Breadth of a box
      double height;      // Height of a box
};

class Prune : public Essential{
   public:
     FunctionA();

   private:
      double l;     

};

class Traverse : public Essential{
   public:
      FunctionB();
      
   private:
      double h;     
};

class Graph {
   public:
      Prune getPrune()
      {
         return *p;
      }

      Traverse getTraverse()
      {
         return *t;
      }
      
   private:
      Prune *p;
      Traverse *t;
};

```

## Reference

[Interfaces in C++ (Abstract Classes)](https://www.tutorialspoint.com/cplusplus/cpp_interfaces.htm)

[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)