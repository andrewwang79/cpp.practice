#include <exception>
#include <iostream>

#include "Logger.h"

using namespace std;
using namespace log4cplus;

IMPLEMENT_LOGGER(main)

int main() {
  // 初始化
  log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_STRING_TO_TSTRING("./conf/logger.cfg"));

  // 示例
  LOG_INFO("log demo. Hello/你好, World. This is log demo.");
  string code = "111";
  // 带参数的格式：标题 : key1[value1], keyX[valueX]
  LOG_DEBUG_FMT("result : code[%s], length[%d]", code.c_str(), 100);
  // 异常
  try {
    throw std::invalid_argument("Unknown Exception");
  } catch (const std::exception& ex) {
    // cout << ex.what() << endl;
    LOG_EXCEPTION("abc", ex);
  }

  LOG_INFO("press any key to quit.");
  getchar();
  return 0;
}
