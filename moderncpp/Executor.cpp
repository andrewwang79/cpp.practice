#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "ExecPool.h"

void test() {
  std::vector<ExecTask<double>> tasks;
  tasks.push_back(ExecTask<double>(
      [](const std::atomic_bool& stop) {
        std::cout << "[Executor-taskfn] task 1 begin" << std::endl;

        auto start_time = std::chrono::system_clock::now();
        int runMS = 1000;
        int sleepMS = 10;
        for (int i = 0; i < runMS / sleepMS; i++) {
          if (stop) {
            std::cout << "[Executor-taskfn] task 1 end by stop" << std::endl;
            return ExecResult<double>(1, 0);
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
        }

        std::cout << "[Executor-taskfn] task 1 end" << std::endl;
        return ExecResult<double>(0, 1.0);
      },
      "1", false));
  tasks.push_back(ExecTask<double>(
      [](const std::atomic_bool& stop) {
        std::cout << "[Executor-taskfn] task 2 begin" << std::endl;

        auto start_time = std::chrono::system_clock::now();
        int runMS = 3000;
        int sleepMS = 10;
        for (int i = 0; i < runMS / sleepMS; i++) {
          if (stop) {
            std::cout << "[Executor-taskfn] task 2 end by stop" << std::endl;
            return ExecResult<double>(1, 0);
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
        }

        std::cout << "[Executor-taskfn] task 2 end" << std::endl;
        return ExecResult<double>(0, 1.2);
      },
      "2", true));
  tasks.push_back(ExecTask<double>(
      [](const std::atomic_bool& stop) {
        std::cout << "[Executor-taskfn] task 3 begin" << std::endl;

        auto start_time = std::chrono::system_clock::now();
        int runMS = 2500;
        int sleepMS = 10;
        for (int i = 0; i < runMS / sleepMS; i++) {
          if (stop) {
            std::cout << "[Executor-taskfn] task 3 end by stop" << std::endl;
            return ExecResult<double>(1, 0);
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
        }

        std::cout << "[Executor-taskfn] task 3 end" << std::endl;
        return ExecResult<double>(0, 1.2);
      },
      "3", false));

  ExecPool<double> pool(
      tasks,
      [&](const std::string& code, const ExecResult<double>& er) {
        std::cout << "[Executor-pool] task(" << code << ") executed : code[" << er.retCode << "]"
                  << ", result[" << er.result << "]" << std::endl;
        pool.stop();
      },
      2000, [&]() { std::cout << "[Executor-pool]timeout task: " << pool.getSuccessTaskCount() << std::endl; });
  std::cout << "[Executor-pool] run begin" << std::endl;
  pool.run();
  std::cout << "[Executor-pool] run result : total[" << pool.getTaskCount() << "]"
            << ", success[" << pool.getSuccessTaskCount() << "]" << std::endl;
  std::cout << "[Executor-pool] run end" << std::endl;
}
int main() {
  test();
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));  // 看detach的线程回调是否会异常
  return 0;
}