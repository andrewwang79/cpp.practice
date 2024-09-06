#include <algorithm>
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief 任务池，有timeout任务（使用者可以关闭所有任务）
 * @attention log statement prefix: [Executor-
 */

std::string _spec_time(const std::chrono::time_point<std::chrono::system_clock>& tm) {
  auto now_c = std::chrono::system_clock::to_time_t(tm);

  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tm.time_since_epoch()) % 1000;
  std::string result(buffer);
  result += "." + std::to_string(ms.count());

  return result;
}
std::string _current_time() { return _spec_time(std::chrono::system_clock::now()); }

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

/**
 * @brief 任务
 */
template <typename T>
class ExecTask {
  typedef std::function<ExecResult<T>(const std::atomic_bool&)> TaskFn;
  typedef std::function<void(const std::string&, const ExecResult<T>&)> TaskExecutedFn;

 public:
  ExecTask() {}

  ExecTask(TaskFn _func, const std::string& _code, bool _detach) { init(_func, _code, _detach); }

  ExecTask(const ExecTask<T>& other) {
    code = other.code;
    func = other.func;
    finish = other.finish;
    result = other.result;
    detach = other.detach;
    stopFlag.store(other.stopFlag);
  }

  void init(TaskFn _func, const std::string& _code, bool _detach) {
    code = _code;
    func = _func;
    detach = _detach;
    stopFlag.store(false);
  }

  void run(TaskExecutedFn tef) {
    startTime_ = std::chrono::system_clock::now();
    thread_ = std::thread([this, tef]() {
      try {
        std::cout << "[Executor-ExecTask] task(" << code << ") run : fn begin" << std::endl;
        result = func(stopFlag);
        finish = true;
        endTime_ = std::chrono::system_clock::now();
        std::cout << "[Executor-ExecTask] task(" << code << ") run : fn end" << std::endl;
        // 完成后的处理
        bool handleAfterComplete = (!detach || (detach && !stopFlag));
        if (handleAfterComplete) {
          if (tef) {  // detach&&stop时，tef可能不存在
            std::cout << "[Executor-ExecTask] task(" << code << ") run : tef begin" << std::endl;
            tef(code, result);
            std::cout << "[Executor-ExecTask] task(" << code << ") run : tef end" << std::endl;
          } else
            std::cout << "[Executor-ExecTask] task(" << code << ") run : tef is null" << std::endl;
        }
      } catch (const std::system_error& e) { std::cerr << "[Executor-ExecTask] task(" << code << ") run system_error : " << e.what() << std::endl; } catch (const std::exception& e) {
        std::cerr << "[Executor-ExecTask] task(" << code << ") run exception : " << e.what() << std::endl;
      } catch (...) { std::cerr << "[Executor-ExecTask] task(" << code << ") run exception" << std::endl; }
      std::cout << "[Executor-ExecTask] task(" << code << ") run end : " << getStatus() << std::endl;
    });
  }

  void join() {
    if (thread_.joinable()) thread_.join();
  }

  bool stop() {
    try {
      stopFlag.store(true);
      if (detach && thread_.joinable()) thread_.detach();
      return true;
    } catch (const std::system_error& e) { std::cerr << "[Executor-ExecTask] task(" << code << ") stop system_error : " << e.what() << std::endl; } catch (const std::exception& e) {
      std::cerr << "[Executor-ExecTask] task(" << code << ") stop exception : " << e.what() << std::endl;
    } catch (...) { std::cerr << "[Executor-ExecTask] task(" << code << ") stop exception" << std::endl; }
    return false;
  }

  // 执行总耗时(毫秒)
  long long getRunMS() {
    auto elapsed = endTime_ - startTime_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  }

  std::string getStatus() {
    std::stringstream ss;
    ss << "code[" << code << "], time[" << getRunMS() << "ms], begin[" << _spec_time(startTime_) << "], end[" << _spec_time(endTime_) << "], finish[" << finish << "], stop[" << stopFlag << "], retCode[" << result.retCode << "]";
    return ss.str();
  }

 public:
  ExecTask<T>& operator=(const ExecTask<T>& other) { return *this; }

 public:
  std::string code;           // 任务编码
  TaskFn func;                //  执行函数
  bool finish = false;        // 执行完成flag
  ExecResult<T> result;       // 执行结果
  bool detach;                // detach，不能stop的用detach。必须初始化
  std::atomic_bool stopFlag;  // 执行停止。必须初始化，默认是false
 private:
  std::thread thread_;                                            //  执行线程
  std::chrono::time_point<std::chrono::system_clock> startTime_;  // 执行开始时间
  std::chrono::time_point<std::chrono::system_clock> endTime_;    // 执行结束时间
};

/**
 * @brief 任务池，支持多个任务，两种类型：
 * 标准任务：pool里有N个。执行到结束会自动退出，任务自身可以通过轮询监听pool的stop标志位来提前停止自己
 * 超时任务：pool里只有1个。结束时不会自动停止pool的标准任务，可以代码让pool退出[pool.stop();]
 *
 * @tparam T 所有任务的数据结构
 */
template <typename T>
class ExecPool {
  typedef std::function<void(const std::string&, const ExecResult<T>&)> TaskCallbackFn;
  typedef std::function<void()> TimeoutCallbackFn;

 public:
  ExecPool(){};

  /**
   * @brief Construct a new Exec Pool object
   *
   * @param _tasks 标准任务列表
   * @param _taskCallback 标准任务执行完毕后的回调函数
   * @param _timeoutMS 超时任务毫秒数
   * @param _timeoutCallback 超时任务执行完毕后的回调函数
   */
  ExecPool(const std::vector<ExecTask<T>>& _tasks, TaskCallbackFn _taskCallback, int _timeoutMS, TimeoutCallbackFn _timeoutCallback) { init(_tasks, _taskCallback, _timeoutMS, _timeoutCallback); }

  /**
   * @brief 初始化
   *
   * @param _tasks 标准任务列表
   * @param _taskCallback 标准任务执行完毕后的回调函数
   * @param _timeoutMS 超时任务毫秒数
   * @param _timeoutCallback 超时任务执行完毕后的回调函数
   */
  void init(const std::vector<ExecTask<T>>& _tasks, TaskCallbackFn _taskCallback, int _timeoutMS, TimeoutCallbackFn _timeoutCallback) {
    tasks = _tasks;
    taskCallback = _taskCallback;
    timeoutMS = _timeoutMS;
    timeoutCallback_ = _timeoutCallback;
    stop_ = false;
  }

  void run() {
    startTime_ = std::chrono::system_clock::now();

    runTimeoutTask();

    // 执行所有任务
    for (ExecTask<T>& task : tasks) task.run([this](const std::string& code, const ExecResult<T>& er) { onTaskExecuted(code, er); });

    // 同步等待所有任务完成
    std::cout << "[Executor-pool] task join begin" << std::endl;
    for (ExecTask<T>& task : tasks) task.join();
    if (!timeoutTask.finish) timeoutTask.stop();  // 确保所有任务都执行完毕时，定时器任务能停止
    timeoutTask.join();
    std::cout << "[Executor-pool] task join end" << std::endl;

    endTime_ = std::chrono::system_clock::now();
    std::cout << "[Executor-pool] run end" << std::endl << getStatus();
  }

  bool stop() {
    std::cout << "[Executor-pool] stop" << std::endl;
    if (stop_) return true;
    stop_ = true;

    std::cout << "[Executor-pool] timeoutTask stop begin" << std::endl;
    bool timeoutTaskStopResult = timeoutTask.stop();
    std::cout << "[Executor-pool] timeoutTask stop end : " << timeoutTaskStopResult << std::endl;

    bool stopResult = true;
    for (ExecTask<T>& task : tasks) {
      std::cout << "[Executor-pool] task stop begin(" << task.code << ")" << std::endl;
      bool taskStopResult = task.stop();
      std::cout << "[Executor-pool] task stop end(" << task.code << ") : " << taskStopResult << std::endl;
      if (!taskStopResult) stopResult = false;
    }
    return stopResult;
  }

  // 执行总耗时(毫秒)
  long long getRunMS() {
    auto elapsed = endTime_ - startTime_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  }

  // 执行到现在的耗时(毫秒)
  long long getCurrentRunMS() {
    auto elapsed = std::chrono::system_clock::now() - startTime_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  }

  size_t getTaskCount() { return tasks.size(); }

  int getFinishTaskCount() { return successTaskCount_ + failTaskCount_; }

  int getSuccessTaskCount() { return successTaskCount_; }

  int getFailTaskCount() { return failTaskCount_; }

  std::string getStatus() {
    std::stringstream ss;
    ss << "  time[" << getRunMS() << "ms], begin[" << _spec_time(startTime_) << "], end[" << _spec_time(endTime_) << "], total[" << getTaskCount() << "], success[" << getSuccessTaskCount() << "], fail[" << getFailTaskCount() << "]" << std::endl;
    for (ExecTask<T>& task : tasks) ss << "  " << task.getStatus() << std::endl;
    ss << "  timeout task : " << timeoutTask.getStatus() << std::endl;
    return ss.str();
  }

 private:
  void runTimeoutTask() {
    timeoutTask.init(
        [this](const std::atomic_bool& stop) {
          // 执行(设置定时任务)任务，每100毫秒检查是否要终止[sleep时间太小时，线程切换次数增多，会严重影响总时间]
          std::cout << "[Executor-pool] task timeout schedule begin" << std::endl;
          int sleepMS = 100;
          for (int i = 0; i < timeoutMS / sleepMS; i++) {
            if (stop) {
              std::cout << "[Executor-pool] task timeout schedule end by stop" << std::endl;
              return ExecResult<bool>(0, false);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
          }
          std::cout << "[Executor-pool] task timeout schedule end" << std::endl;
          return ExecResult<bool>(0, true);
        },
        "TIMEOUT", false);
    timeoutTask.run([this](const std::string& code, const ExecResult<bool>& er) {
      std::cout << "[Executor-pool] task timeout begin" << std::endl;
      timeoutCallback_();
      std::cout << "[Executor-pool] task timeout end : time[" << getCurrentRunMS() << "]" << std::endl;
    });
  }

  void onTaskExecuted(const std::string& taskCode, const ExecResult<T>& result) {
    if (result.retCode == 0)
      successTaskCount_++;
    else
      failTaskCount_++;
    taskCallback(taskCode, result);
  }

 public:
  ExecTask<bool> timeoutTask;      // 定时器的任务，不计入任务数
  std::vector<ExecTask<T>> tasks;  // 所有的任务
  TaskCallbackFn taskCallback;     // 任务执行完毕后的回调函数
  int timeoutMS;                   // 定时器毫秒

 private:
  std::chrono::time_point<std::chrono::system_clock> startTime_;  // 执行开始时间
  std::chrono::time_point<std::chrono::system_clock> endTime_;    // 执行结束时间
  std::atomic_bool stop_;                                         // 执行停止

  TimeoutCallbackFn timeoutCallback_;  //  定时器的回调函数

  int successTaskCount_ = 0;  // 成功的任务数
  int failTaskCount_ = 0;     // 失败的任务数
};
