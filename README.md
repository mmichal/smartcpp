#SmartCpp

A simple C++ smart pointer with reference counting

###C++ is a low level language with high level OO model that nobody understands.

Writing a neat, secure and readable code using C++ is very hard and requires great planning skills. You have to think more about how to write your code, than what you want to write. One of the biggest problems of C++ is a lack of garbage collection. Almost every modern programming language has at least refflection API, that makes writing a garbage collector possible. The latest C++ standard was published in 2003 - almost 8 years ago - and even planned C++0x won't make memory menagment easier. Is there any hope for programmers that have girlfriends and social lives? Actually there is!


###Smart people ivented smart pointers.

Smart pointer is a statically allocated object that holds a pointer to dynamically allocated data. The problem with dynamic data is that you loose your pointer when it comes out of range, but data still exists in a memory. That's a memory leak - the data is inaccsessable. You can't read, write or remove it. So you have to delete the data manually before the pointer is deleted, or do it automaticaly when pointer comes out of range. That's why smart pointer is a statically allocated OBJECT. Classes have destructors that are called before instance is deleted. So when smart pointer comes out of range, its destructor can delete dynamic data. Actually there is a smart pointer implementation in C++ stdlib in a memory library. But it doesn't implement reference counting.

What does it mean? Imagine that you have class Car that has a public field enigne of type Engine* and class CarService with one public method putEngineInACar(Car&amp;). As you can see putEngineInACar has one parameter that is a reference to a Car object. It takes that reference. Creates an auto_ptr that points to dynamically allocated Engine and it fills the engine field of Car instance. Now we have two pointers that points to the same Engine instance. What happens when auto_ptr comes out of range (in other words - when putEngineInACar returns). The Engine instance will be delated and engine field in Car instance will point to nowhere. That's Why we need a reference counter. A counter that holds number of fields that point to Engine instance so the instance is only deleted when counter equals 0.

###A counter you can count on.

There is no such smart pointer in C++ stdlib. There is one in Boost, but it's just a one feature and there is no need to add such big dependecy to the project because of it. So let's write our own.

We need two class templates. One that holds a pointer to our value and reference counter. And one that will be our reference instance.

This one holds pointer:

```c++
/*
 * File:   PointerContainer.hpp
 * Author: michamaciejewski
 *
 */

#ifndef _POINTERCONTAINER_HPP
#define _POINTERCONTAINER_HPP

#include <list>
using namespace std;

template <class Type> class PointerContainer {
  public:
    Type* getElement() {
      return this->element;
    }

    PointerContainer(Type* element) : referenceCounter(1) {
      this->element = element;
    }

    ~PointerContainer() {
      delete this->element;
    }

    void increment() {
      this->referenceCounter++;
    }

    void decrement() {
      this->referenceCounter--;
    }

    int getCounter() {
      return this->referenceCounter;
    }

  protected:
    int referenceCounter;
    Type* element;
    //list<PointerContainer<void> >::iterator iterator;
};

#endif /* _POINTERCONTAINER_HPP */
```
And this one is our reference instance template:

```c++
/*
 * File:   Smart.hpp
 * Author: michamaciejewski
 *
 */

#ifndef _SMART_HPP
#define _SMART_HPP

#include "PointerContainer.hpp"
template <class Type> class Smart {
public:
  Smart() : container(0) {
  }

  Smart(const Smart<Type>& object) {
    this->container = object.container;
    this->container->increment();
  }

  Smart(Type* element) {
    this->container = new PointerContainer<Type> (element);
  }

  ~Smart() {
    this->removeReference();
  }

  bool operator==(const Smart<Type>& other) const {
    return this->container == other.container;
  }

  bool operator!=(constother) const {
    return this->container != other.container;
  }

  Type* operator->() {
    return this->container->getElement();
  }

  Type* operator->() const {
    return this->container->getElement();
  }


  Smart<Type>& operator=(const Smart<Type>& object) {
    if (this->container != 0) {
      this->removeReference();
    }
    this->container = object.container;
    this->container->increment();
    return *this;
  }

  Type& operator*() {
    return *this->container->getElement();
  }

protected:
  void removeReference() {
    if (this->container != 0) {
      this->container->decrement();
      if (this->container->getCounter() == 0) {
        delete this->container;
      }
    }
  }

  PointerContainer<Type>* container;
};

#endif // _SMART_HPP
```

As you can see it's all about reference couting. With some operators overloaded in Smart template we can use it in very easy and neat way.

Lets say we write a 3d game engine in C++. We have the Vertex class. It's constructor takes three parameters - x, y and z. It's quite obvious that Vertex represents point in a 3d space. That's how we can instantiate it:

```c++
Smart<Vertex> vertex1(new Vertex(10.00, 10.00, 0.0));
Smart<Vertex> vertex2(new Vertex(1.00, 1.00, 0.0));
```

Because we have overloaded operator ->. We can do this:
```C++
vertex1->setColor(red);
```

Until now both Vertex instances had a reference counter equal to 1.

Ok... So what will happen when we'll do:

```c++
vertex1 = vertex2;
```

We have overloaded = operator. So the reference counter of vertex1 is decremented by 1. Because now it is equal to 0 the instance is deleted. The RC of vertex2 is incremented by 1. And vertex2 reference is copied to vertex1. As you can see when vertex1 and vertex2 comes out of the scope their reference counters are also decremented. That's it!

This is one of the simplest forms of garbage collection. So simple that some people doesn't even want to call it garbage collection. Not without a reason. It's not universal solution. For example it doesn't handle circular references but still... it works! :-)

