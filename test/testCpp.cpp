#include "bits/stdc++.h"

class MyVirtualBase {
 public:
  virtual void test(int x) = 0;
};

class MyImpl_1 : public MyVirtualBase {
 public:
  void test(int x) { printf("%d\n", x); }
};

int main() {
  MyImpl_1 myImpl_1;
  myImpl_1.test(1);
  return 0;
}