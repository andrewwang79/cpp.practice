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

  ExecResult() {}

  ExecResult(int _retCode, T _value) {
    retCode = _retCode;
    result = _value;
  }
  ExecResult(const ExecResult<T>& other) {
    retCode = other.retCode;
    result = other.result;
  }
};

template <typename T>
class ExecTask {
  typedef std::function<ExecResult<T>(const std::atomic_bool&)> TaskFn;
  typedef std::function<void(const std::string&, const ExecResult<T>&)> TaskExecutedFn;

 public:
  ExecTask(TaskFn _func, const std::string& _code, bool _detach) {
    code = _code;
    func = _func;
    detach = _detach;
  }

  ExecTask(const ExecTask<T>& other) {
    code = other.code;
    func = other.func;
    result = other.result;
    detach = other.detach;
  }

  void run(TaskExecutedFn tef) {
    thread_ = std::thread([this, tef]() {
      std::cout << "[Executor-ExecTask] task(" << code << ") begin " << std::endl;
      result = func(stop_);
      if (!stop_) tef(code, result);
      std::cout << "[Executor-ExecTask] task(" << code << ") end " << std::endl;
    });
  }

  void join() {
    if (thread_.joinable()) thread_.join();
  }

  void stop() {
    stop_ = true;
    if (detach) thread_.detach();
  }

 public:
  ExecTask<T>& operator=(const ExecTask<T>& other) { return *this; }

 public:
  std::string code;      // 任务编码
  TaskFn func;           //  执行函数
  ExecResult<T> result;  // 执行结果
  bool detach;           // detach，不能stop的用detach
 private:
  std::thread thread_;     //  执行线程
  std::atomic_bool stop_;  // 执行停止
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
      std::cout << "[Executor-pool] task timeout begin" << std::endl;
      auto start_time = std::chrono::system_clock::now();
      int sleepMS = 10;
      for (int i = 0; i < timeoutMS / sleepMS; i++) {
        if (stop_) {
          std::cout << "[Executor-pool] task timeout end by stop" << std::endl;
          return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
      }
      timeoutCallback_();
      std::cout << "[Executor-pool] task timeout end" << std::endl;
    });

    // 执行所有任务
    for (ExecTask<T>& task : tasks) task.run([this](const std::string& code, const ExecResult<T>& er) { onTaskExecuted(code, er); });

    // 同步等待所有任务完成
    std::cout << "[Executor-pool] task wait begin" << std::endl;
    for (ExecTask<T>& task : tasks) task.join();
    if (timeoutThread.joinable()) timeoutThread.join();
    std::cout << "[Executor-pool] task wait end" << std::endl;
  }

  void stop() {
    std::cout << "[Executor-pool] stop" << std::endl;
    stop_ = true;
    for (ExecTask<T>& task : tasks) task.stop();
  }

  void onTaskExecuted(const std::string& taskCode, const ExecResult<T>& result) {
    if (result.retCode == 0)
      successTaskCount_++;
    else
      failTaskCount_++;
    taskCallback(taskCode, result);
  }

  // 执行时间(毫秒)
  double getRunMS() {
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
