#include "MyDebugLibClass.h"

#include <iostream>

MyDebugLibClass::MyDebugLibClass() {
  // 构造函数的实现
}

int MyDebugLibClass::add(int a, int b) {
  std::cout << "add has been called" << std::endl;
  return a + b;
}

extern "C" __declspec(dllexport) int Add(int a, int b) {
  std::cout << "Add has been called" << std::endl;
  return a + b;
}
