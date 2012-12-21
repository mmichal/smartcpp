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

