#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class ExecResult {
 public:
  int retCode;
  T result;

  ExecResult(int _retCode, T _value) {
    retCode = _retCode;
    result = _value;
  }
};

template <typename T>
class ExecWrapper {
 public:
  ExecWrapper(std::function<ExecResult<T>()> func) { func_ = func; }

  void run() { asyncFunc_ = std::async(std::launch::async, func_); }

  // 等待异步任务完成，并获取返回值
  ExecResult<T> wait() { return asyncFunc_.get(); }

 public:
  ExecWrapper(const ExecWrapper<T>& other) {}

  ExecWrapper<T>& operator=(const ExecWrapper<T>& other) { return *this; }

 private:
  std::future<ExecResult<T>> asyncFunc_;
  std::function<ExecResult<T>()> func_;
};

template <typename T>
class ExecPool {
 public:
  std::vector<ExecWrapper<T>> wrappers;

  std::function<void(const ExecResult<T>&)> itemCallback;
  int timeoutMS;  // 毫秒
  std::function<void()> timeoutCallback;

  std::chrono::time_point<std::chrono::system_clock> start_time;

  ExecPool(const std::vector<ExecWrapper<T>>& _wrappers, std::function<void(const ExecResult<T>&)> _itemCallback, int _timeoutMS, std::function<void()> _timeoutCallback) {
    wrappers = _wrappers;
    itemCallback = _itemCallback;
    timeoutMS = _timeoutMS;
    timeoutCallback = _timeoutCallback;
  }

  void run() {
    start_time = std::chrono::system_clock::now();
    std::thread([this]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMS));
      timeoutCallback();
    }).detach();
    for (auto& wrapper : wrappers) wrapper.run();

    std::vector<std::future<void>> futures;
    for (auto& wrapper : wrappers) {
      futures.emplace_back(std::async(std::launch::async, [&]() {
        ExecResult<T> result = wrapper.wait();
        if (result.retCode == 0) {
          successItemCount_++;
          itemCallback(result);
        } else
          failItemCount_++;
      }));
    }

    // 等待所有异步任务完成
    for (auto& future : futures) { future.wait(); }
  }

  int getSuccessItemCount() { return successItemCount_; }

 private:
  int successItemCount_ = 0;
  int failItemCount_ = 0;
};

int main() {
  std::vector<ExecWrapper<double>> wrappers;
  wrappers.push_back(ExecWrapper<double>([]() {
    std::cout << "Wrapper 1 begin" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(7));
    std::cout << "Wrapper 2 end" << std::endl;
    return ExecResult<double>(0, 1.0);
  }));
  wrappers.push_back(ExecWrapper<double>([]() {
    std::cout << "Wrapper 2 begin" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Wrapper 2 end" << std::endl;
    return ExecResult<double>(0, 1.2);
  }));

  ExecPool<double> pool(
      wrappers, [&](const ExecResult<double>& er) { std::cout << "Executed task with result: " << er.result << std::endl; }, 5000,
      [&]() { std::cout << "timeout: " << pool.getSuccessItemCount() << std::endl; });
  pool.run();

  return 0;
}
