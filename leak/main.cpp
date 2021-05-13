#include <iostream>
using namespace std;

class Abc {
  int xyz = 0;
};

void classLeak() { Abc* abc = new Abc(); }

void charLeak() {
  char* p = new char[10];    // vs 编译器则不进行初始化
  char* q = new char[10]();  // vs 编译器将其初始化为0

  cout << "p:" << p << endl;
  cout << "q:" << q << endl;
  delete p;
}

int main(int argc, char* argv[]) {
  classLeak();
  return 0;
}
