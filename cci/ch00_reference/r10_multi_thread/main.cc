/*           
 * Title        :   Reference10. Milti thread with c++11
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :   Concurrent Queue with condition variable
 *
 * Memory Leak Cheking Method:
 * valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./run
 *
 * Ref:
 * http://www.gamasutra.com/view/feature/4006/sponsored_feature_omg_.php?page=4
 * http://www.codeproject.com/Articles/598695/Cplusplus-threads-locks-and-condition-variables
 * http://codereview.stackexchange.com/questions/84109/a-multi-threaded-producer-consumer-with-c11
 */
#include <chrono>
#include "concurrent_queue.h"

void producer(ConcurrentQueue<int>* q, unsigned long count)
{
#ifdef DEBUG_ENABLED  
  std::cout << "producer: ++" << std::endl;
#endif  

  for (unsigned long i = 0; i < count; ++i)
  {
    q->push(rand());
  }

#ifdef DEBUG_ENABLED  
  std::cout << "producer: --" << std::endl;
#endif  
}

void consumer(ConcurrentQueue<int>* q, unsigned long count)
{
#ifdef DEBUG_ENABLED  
  std::cout << "consumer: ++" << std::endl;
#endif  

  for (unsigned long i = 0; i < count; ++i)
  {
    int x;
    q->wait_and_pop(x);
    //q->try_pop(x); /* no wait */
  }

#ifdef DEBUG_ENABLED  
  std::cout << "consumer: --" << std::endl;
#endif  
}

void complex_worker(ConcurrentQueue<int>* q, unsigned long count)
{
#ifdef DEBUG_ENABLED  
  std::cout << "complex_worker: ++" << std::endl;
#endif  

  for (unsigned long i = 0; i < count; ++i)
  {
    q->push(rand());
  }
  for (unsigned long i = 0; i < count; ++i)
  {
    int x;
    q->wait_and_pop(x);
   }

#ifdef DEBUG_ENABLED  
  std::cout << "complex_worker: --" << std::endl;
#endif  
}

int main()
{
  std::cout << "\nmain: Create queue ---------------------------" << std::endl;
  ConcurrentQueue<int> q;
  unsigned long const count = 100000;
  std::chrono::steady_clock::time_point start, stop;

#if 1
  std::cout << "\nmain: Test with main thread ------------------" << std::endl;
  start = std::chrono::steady_clock::now();
  producer(&q, count);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1));
  consumer(&q, count);
  stop = std::chrono::steady_clock::now();
  std::cout<<"main: time taken "
    << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count()
    << "ms" << std::endl;
#endif

#if 1
  std::cout << "\nmain: Test by complex thread -----------------" << std::endl;
  std::thread complex_thread(complex_worker, &q, count);
  start = std::chrono::steady_clock::now();
  complex_thread.join();
  stop = std::chrono::steady_clock::now();
  std::cout<<"main: time taken "
    << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count()
    << "ms" << std::endl;
#endif

#if 1
  std::cout << "\nmain: Test by divide thread ------------------" << std::endl;
  std::thread producer_thread(producer, &q, count);
  std::thread consumer_thread(consumer, &q, count);
  start = std::chrono::steady_clock::now();
  producer_thread.join();
  consumer_thread.join();
  stop = std::chrono::steady_clock::now();
  std::cout<<"main: time taken "
    << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count()
    << "ms" <<std::endl;
#endif

#if 1
  std::cout << "\nmain: Test vy 5 consumer thread --------------" << std::endl;
  std::thread producer_thread1(producer, &q, count);
  std::thread consumer_thread1(consumer, &q, count/5);
  std::thread consumer_thread2(consumer, &q, count/5);
  std::thread consumer_thread3(consumer, &q, count/5);
  std::thread consumer_thread4(consumer, &q, count/5);
  std::thread consumer_thread5(consumer, &q, count/5);
  start = std::chrono::steady_clock::now();
  producer_thread1.join();
  consumer_thread1.join();
  consumer_thread2.join();
  consumer_thread3.join();
  consumer_thread4.join();
  consumer_thread5.join();
  stop = std::chrono::steady_clock::now();
  std::cout<<"main: time taken "
    << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count()
    << "ms" <<std::endl;
#endif

  std::cout << "\nmain: Program exit ---------------------------" << std::endl;

  return 0;
}
