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


