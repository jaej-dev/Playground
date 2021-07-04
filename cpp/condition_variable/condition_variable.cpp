/*
 * Title      :   condition_variable.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <atomic>

std::condition_variable g_cv;
std::mutex g_mutex;
int g_count = 0;

std::atomic_flag g_lock = ATOMIC_FLAG_INIT;

void waits(int num) {
  int count = 0;
  std::unique_lock<std::mutex> lk(g_mutex);
  std::cout << "thread " << num << ": waiting...\n";
  g_cv.wait(lk, [&count, num]() {
    count++;
    std::cout << "thread " << num << ": execute count = " << count << "\n";
    return g_count == 3;
  });
  std::cout << "finished waiting!\n";
}

void signals(int num) {
  for (int i = 1; i < 5; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
      std::lock_guard<std::mutex> lk(g_mutex);
      g_count = i;
      std::cout << "\nthread " << num << ": g_count is " << g_count
                << ", nofitying now...\n\n";
    }
    g_cv.notify_all();
  }
  std::cout << "finished signaling!\n";
}

void counter_safe(int num) {
  std::cout << "start counter_safe thread " << num << "\n";

  for (int i = 0; i < 50; i++){
    while (g_lock.test_and_set(std::memory_order_acquire));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "thread " << num << ": count = " << i << std::endl;
    g_lock.clear(std::memory_order_release);
  }
  
  std::cout << "finished counter_safe thread " << num << std::endl;
}

void counter_unsafe(int num) {
  std::cout << "start counter_unsafe thread " << num << "\n";

  for (int i = 0; i < 50; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "thread " << num << ": count = " << i << std::endl;
  }

  std::cout << "finished counter_unsafe thread " << num << std::endl;
}

int main(int argc, char* argv[]) {
  std::cout << "program start...\n";
#if 0  
  std::cout << "\n--- test for condition variable ---\n";
  std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals, 4);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
#endif
  std::cout << "\n--- test for counter with atomic flag ---\n";
  std::thread t5(counter_safe, 5), t6(counter_safe, 6);
  t5.join();
  t6.join();

  std::cout << "\n--- test for counter without atomic flag ---\n";
  std::thread t7(counter_unsafe, 7), t8(counter_unsafe, 8);
  t7.join();
  t8.join();

  std::cout << "program exit...\n";
  return 0;
}
