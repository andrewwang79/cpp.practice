#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "ExecPool.h"

void test() {
  int sleepMS = 100;  // sleep时间太小时，线程切换次数增多，会严重影响总时间
  std::vector<ExecTask<double>> tasks;
  tasks.push_back(ExecTask<double>([&sleepMS](const std::atomic_bool& stop) {
    std::cout << "[Executor-biz] task 1 begin" << std::endl;
    auto start_time = std::chrono::system_clock::now();
    int runMS = 1000;
    for (int i = 0; i < runMS / sleepMS; i++) {
      if (stop) {
        std::cout << "[Executor-biz] task 1 end by stop" << std::endl;
        return ExecResult<double>(1, 0);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
    }
    std::cout << "[Executor-biz] task 1 end" << std::endl;
    return ExecResult<double>(0, 1.1);
  }, "1", false));
  tasks.push_back(ExecTask<double>([&sleepMS](const std::atomic_bool& stop) {
    std::cout << "[Executor-biz] task 2 begin" << std::endl;
    auto start_time = std::chrono::system_clock::now();
    int runMS = 2000;
    for (int i = 0; i < runMS / sleepMS; i++) {
      if (stop) {
        std::cout << "[Executor-biz] task 2 end by stop" << std::endl;
        return ExecResult<double>(1, 0);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
    }
    std::cout << "[Executor-biz] task 2 end" << std::endl;
    return ExecResult<double>(0, 1.2);
  }, "2", true));
  tasks.push_back(ExecTask<double>([&sleepMS](const std::atomic_bool& stop) {
    std::cout << "[Executor-biz] task 3 begin" << std::endl;
    auto start_time = std::chrono::system_clock::now();
    int runMS = 4000;
    for (int i = 0; i < runMS / sleepMS; i++) {
      if (stop) {
        std::cout << "[Executor-biz] task 3 end by stop" << std::endl;
        return ExecResult<double>(1, 0);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
    }
    std::cout << "[Executor-biz] task 3 end" << std::endl;
    return ExecResult<double>(0, 1.3);
  }, "3", false));

  ExecPool<double> pool;
  pool.init(tasks,
            [&](const std::string& code, const ExecResult<double>& er) {
              std::cout << "[Executor-biz] task(" << code << ") executed : code[" << er.retCode << "], result[" << er.result << "]" << std::endl;
              // pool.stop();
            },
            3000, [&]() { std::cout << "[Executor-biz] timeout executed" << std::endl; });
  std::cout << "[Executor-biz] run begin" << std::endl;
  pool.run();
  std::cout << "[Executor-biz] run end" << std::endl;
}

int main() {
  test();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));  // 超过detach线程的执行时间，检测其回调是否异常
  return 0;
}