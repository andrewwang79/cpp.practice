#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
bool thread1_done = false;
bool thread2_done = false;
bool stop_threads = false;

void thread_function(int id) {
  std::this_thread::sleep_for(std::chrono::seconds(id));  // 模拟不同的工作时间
  {
    if (id == 1) {
      thread1_done = true;
      //   std::this_thread::sleep_for(std::chrono::seconds(7));
    } else {
      thread2_done = true;
    }
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << id << " lock." << std::endl;
    stop_threads = true;
    std::cout << "Thread " << id << " done." << std::endl;
  }
  cv.notify_all();  // 通知主线程
}

void main_thread_function() {
  // cv.wait(lock, 谓词);
  // cv.wait()会自动释放互斥锁lock(内容是mtx)，允许其他线程可以锁定互斥锁并修改共享数据，如谓词里的stop_threads。
  // 当cv被通知时，cv.wait()会重新锁定lock，检查谓词。
  // 如果谓词false，线程会再次进入等待状态，并再次释放lock。
  // 如果谓词true，cv.wait()会返回，线程继续执行后续代码。

  std::unique_lock<std::mutex> lock(mtx);  // 构造锁定mtx，析构释放mtx
  std::cout << "Main thread lock." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(6));
  std::cout << "Main thread weak." << std::endl;
  cv.wait(lock, [] { return stop_threads; });  // 等待任意一个线程结束
  std::cout << "Main thread detected one thread finished, stopping both threads." << thread1_done << "|" << thread2_done << std::endl;
}

void cv_test() {
  std::thread t1(thread_function, 1);
  std::thread t2(thread_function, 2);

  main_thread_function();  // 主线程等待

  if (t1.joinable()) t1.join();
  if (t2.joinable()) t2.join();
}

int main() {
  cv_test();
  return 0;
}