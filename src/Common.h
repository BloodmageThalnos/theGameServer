#pragma once

#include "bits/stdc++.h"

template <class T>
class Singleton {
 public:
  static T& getInstance() {
    if (!tInstance) {
      tInstance = new T();
      atexit(_destroy);
    }
    return *tInstance;
  }

 protected:
  Singleton() {}
  virtual ~Singleton() {}

 private:
  static void _destroy() {
    if (tInstance) {
      delete (tInstance), tInstance = nullptr;
    }
  }
  static T* tInstance;
};

template <typename T>
T* Singleton<T>::tInstance = 0;
