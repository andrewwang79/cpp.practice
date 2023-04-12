// so文件改变前必须先close，用新so来close会内存异常。

#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
using namespace std;

typedef double (*power_func)(double, int);

const char* LIB_PATH = "/usr/lib/libtestLibX.so";

int main() {
  ino_t so_ino = -1;
  void* handle;
  char* error;
  for (;;) {
    string function_name;
    cout << "What function (type 'quit' to exit, 'close' to close, type 'power/power_wrong' to test): ";
    cin >> function_name;
    if (function_name == "quit") break;
    if (function_name == "close") {
      if (handle) { dlclose(handle); }
      continue;
    }

    struct stat attr;
    if (stat(LIB_PATH, &attr) == 0 && attr.st_ino != so_ino) {
      // 动态加载so
      if (handle) { dlclose(handle); }
      handle = dlopen(LIB_PATH, RTLD_LAZY);
      if (!handle) {
        cerr << dlerror() << endl;
        return 1;
      }

      // 获取动态加载函数
      power_func func = reinterpret_cast<power_func>(dlsym(handle, function_name.c_str()));
      if (error = dlerror()) {
        cerr << error << endl;
        continue;
      } else {
        double a;
        int b;
        cout << "Input base number: ";
        cin >> a;
        cout << "Input exponent number: ";
        cin >> b;
        // 执行动态加载函数
        double result = func(a, b);
        cout << result << endl;
      }
    }
  }

  return 0;
}
