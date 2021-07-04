/*           
 * Title        :   Reference10. Milti thread with c++11
 * Filename     :   concurrent_queue.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :   Concurrent Queue with condition variable
 *
 * ref:
 * http://www.gamasutra.com/view/feature/4006/sponsored_feature_omg_.php?page=4
 * http://www.codeproject.com/Articles/598695/Cplusplus-threads-locks-and-condition-variables
 * http://codereview.stackexchange.com/questions/84109/a-multi-threaded-producer-consumer-with-c11
 */

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>

#undef DEBUG_ENABLED
//#define DEBUG_ENABLED

template<typename Data>
class ConcurrentQueue
{
public:
  void push(const Data& data)
  {
#ifndef DEBUG_ENABLED
    {
      std::lock_guard<std::mutex> lk(mutex_);
      queue_.push(data);
    }
    //cv_.notify_one(); /* signal to cv.wait */
    cv_.notify_all();
#else    
    {
      std::lock_guard<std::mutex> lk(mutex_);
      queue_.push(data);
    }
    std::cout << "push: signal..." << std::endl;
    //cv_.notify_one(); /* signal to cv.wait */
    cv_.notify_all();
    std::cout << "push: data = " << data << std::endl;
#endif
  }

  bool try_pop(Data& data)
  {
#ifndef DEBUG_ENABLED
    std::lock_guard<std::mutex> lk(mutex_);
    if (queue_.empty())
      return false;
    data = queue_.front();
    queue_.pop();
    return true;
#else
    std::lock_guard<std::mutex> lk(mutex_);
    if (queue_.empty())
    {
      std::cout << "try_pop: empty, thread = " << std::this_thread::get_id()
        << std::endl; 
      return false;
    }
    data = queue_.front();
    queue_.pop();
    std::cout << "try_pop: data = " << data << std::endl;
    return true;
#endif
  }

  void wait_and_pop(Data& data)
  {
#ifndef DEBUG_ENABLED
    std::unique_lock<std::mutex> lk(mutex_);
    while (queue_.empty())
    {
      cv_.wait(lk); /* wait from cv.notify_one or notify_all */
    }
    data = queue_.front();
    queue_.pop();
#else
    std::unique_lock<std::mutex> lk(mutex_);
    while (queue_.empty())
    {
      std::cout << "thread = " << std::this_thread::get_id() << " is waiting.."
        << std::endl; 
      cv_.wait(lk);
    }
    data = queue_.front();
    queue_.pop();
    std::cout << "wait_and_pop: data = " << data << std::endl;
#endif
  }

private:    
  std::mutex mutex_;
  std::queue<Data> queue_;
  std::condition_variable cv_;
};
