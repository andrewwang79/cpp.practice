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
class ExecTask {
  typedef std::function<ExecResult<T>()> TaskFn;

 public:
  ExecTask(TaskFn _func, const std::string& _code) {
    func = _func;
    code = _code;
  }

  ExecTask(const ExecTask<T>& other) {
    func = other.func;
    code = other.code;
  }

 public:
  ExecTask<T>& operator=(const ExecTask<T>& other) { return *this; }

 public:
  TaskFn func;
  std::string code;
};

template <typename T>
class ExecPool {
  typedef std::function<void(const std::string&, const ExecResult<T>&)> TaskCallbackFn;
  typedef std::function<void()> TimeoutCallbackFn;

 public:
  ExecPool(const std::vector<ExecTask<T>>& _tasks, TaskCallbackFn _taskCallback, int _timeoutMS, TimeoutCallbackFn _timeoutCallback) {
    tasks = _tasks;
    taskCallback = _taskCallback;
    timeoutMS = _timeoutMS;
    timeoutCallback_ = _timeoutCallback;
    stop_ = false;
  }

  void run() {
    startTime_ = std::chrono::system_clock::now();
    // 执行(设置定时任务)任务，每10毫秒检查是否要终止
    std::thread timeoutThread([this]() {
      std::cout << "timeout begin" << std::endl;
      auto start_time = std::chrono::system_clock::now();
      while (!stop_ && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() < timeoutMS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      if (stop_ == false) timeoutCallback_();
      std::cout << "timeout end" << std::endl;
    });

    // 执行所有任务
    // TODO future无法终止，可以改造成thread来终止，需要业务函数支持变量stop。业务函数有几个输入和返回，有中途计算好的返回吗？【看是否使用future】
    for (ExecTask<T>& task : tasks) {
      std::cout << "task begin : " << task.code << std::endl;
      futures_.emplace_back(std::async(std::launch::async, [&]() {
        std::cout << "exec begin : " << task.code << std::endl;
        ExecResult<T> result = task.func();
        std::cout << "exec return : " << task.code << std::endl;
        onTaskExecuted(task.code, result);
        std::cout << "exec end : " << task.code << std::endl;
      }));
      std::cout << "task end : " << task.code << std::endl;
    }

    // 同步等待所有任务完成
    std::cout << "futures ALL begin" << std::endl;
    for (auto& future : futures_) {
      std::cout << "futures begin" << std::endl;
      future.get();
      std::cout << "futures end" << std::endl;
    }
    if (timeoutThread.joinable()) { timeoutThread.join(); }
    std::cout << "futures ALL end" << std::endl;
  }

  void stop() { stop_ = true; }

  void onTaskExecuted(const std::string& taskCode, const ExecResult<T>& result) {
    if (result.retCode == 0) {
      successTaskCount_++;
      taskCallback(taskCode, result);
    } else
      failTaskCount_++;
  }

  // 执行时间(毫秒)
  double getRunMS() const {
    auto elapsed = std::chrono::system_clock::now() - startTime_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  }

  int getTaskCount() { return tasks.size(); }

  int getSuccessTaskCount() { return successTaskCount_; }

 public:
  std::vector<ExecTask<T>> tasks;  // 所有的任务
  TaskCallbackFn taskCallback;     // 每个任务执行完毕后的回调函数
  int timeoutMS;                   // 定时器毫秒

 private:
  std::vector<std::future<void>> futures_;
  std::chrono::time_point<std::chrono::system_clock> startTime_;  // 执行开始时间
  std::atomic_bool stop_;                                         // 执行停止

  std::function<void()> timeoutCallback_;  //  定时器的回调函数

  int successTaskCount_ = 0;  // 成功的任务数
  int failTaskCount_ = 0;     // 失败的任务数
};

int main() {
  std::vector<ExecTask<double>> tasks;
  tasks.push_back(ExecTask<double>(
      []() {
        std::cout << "task 1 begin" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "task 1 end" << std::endl;
        return ExecResult<double>(0, 1.0);
      },
      "1"));
  tasks.push_back(ExecTask<double>(
      []() {
        std::cout << "task 2 begin" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "task 2 end" << std::endl;
        return ExecResult<double>(0, 1.2);
      },
      "2"));

  std::cout << "pool create" << std::endl;
  ExecPool<double> pool(
      tasks,
      [&](const std::string& code, const ExecResult<double>& er) {
        std::cout << "Executed task(" << code << ") with result: " << er.result << std::endl;
        // pool.stop();
      },
      2000, [&]() { std::cout << "timeout: " << pool.getSuccessTaskCount() << std::endl; });
  std::cout << "pool run begin" << std::endl;
  pool.run();
  std::cout << "pool run end" << std::endl;

  return 0;
}
