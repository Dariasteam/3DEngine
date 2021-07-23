#include "multithreadmanager.h"
#include <thread>

void MultithreadManager::calculate_threaded(unsigned size,
                                            const std::function<void (unsigned)>& f) {

  double segment = double(size) / N_THREADS;
  unsigned counter = 0;

  bool finished = false;

  std::mutex local_mtx;
  std::mutex mtx;
  std::condition_variable cv;

  std::function<void(void)> functions [N_THREADS];

  for (unsigned i = 0; i < N_THREADS; i++) {
    functions[i] = [&, segment, i]() {
      unsigned from = std::round(i * segment);
      unsigned to   = std::round((i + 1) * segment);
      for (unsigned j = from; j < to; j++)
        f(j);
      local_mtx.lock();
      counter++;
      unsigned c = counter;
      local_mtx.unlock ();

      if (c == N_THREADS) {
        finished = true;
        cv.notify_one();
      }
    };
    threads[i].send_function(functions[i]);
  }

  std::unique_lock<std::mutex> lck(mtx);
  cv.wait(lck, [&]{return finished;});
}

CallableThread::CallableThread() :
  t (&CallableThread::run, this)
{}

bool CallableThread::send_function(std::function<void ()>& func) {
  f = &func;
  active = true;
  cv.notify_one();
  return true;
}

void CallableThread::end_life() {
  alive = false;
  active = true;
  cv.notify_one();
}

void CallableThread::run() {
  while (alive) {
    {
      std::unique_lock<std::mutex> lck(mtx); // wake up thread
      cv.wait(lck, [&]{return active;});
    }
    active = false;
    (*f) ();
  }
}

const auto& instance = MultithreadManager::get_instance();
