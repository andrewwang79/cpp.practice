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
  if (id == 1) {
    thread1_done = true;
  } else {
    thread2_done = true;
  }
  {
    std::lock_guard<std::mutex> lock(mtx);  // 减少锁的范围和影响
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
  // std::this_thread::sleep_for(std::chrono::seconds(6));
  std::cout << "Main thread weak." << std::endl;
  cv.wait(lock, [] { return stop_threads; });  // 等待任意一个线程结束
  std::cout << "Main thread detected one thread finished, stopping both threads." << thread1_done << "|" << thread2_done << std::endl;
}

void cv_test() {
  // 验证：thread_function如果没有锁保护，且cv.notify_all()通知在主线程cv.wait()之前发生，会导致wait()一直阻塞。但实践中没有出来此情况
  std::thread t1(thread_function, 1);
  std::thread t2(thread_function, 2);

  main_thread_function();

  if (t1.joinable()) t1.join();
  if (t2.joinable()) t2.join();
}

int main() {
  cv_test();
  return 0;
}