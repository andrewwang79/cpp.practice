// libDebugExe.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "MyDebugLibClass.h"

int main() {
  std::cout << "Hello World!\n";
  MyDebugLibClass mdlc;
  std::cout << "Result: " << mdlc.add(49, 51) << std::endl;
  std::cout << "Result: " << Add(2, 3) << std::endl;
  return 0;
}
