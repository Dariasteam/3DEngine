#include "multithreadmanager.h"

void MultithreadManager::calculate_threaded(unsigned size, std::function<void (unsigned)> f) {
  auto lambda = [&](unsigned from, unsigned to) {
    for (unsigned i = from; i < to; i++)
      f(i);
  };

  unsigned segment = size / N_THREADS;
  unsigned counter = 0;

  active = false;

  std::mutex mtx;
  auto callback = [&] () {
    mtx.lock();
    counter++;
    mtx.unlock();
    if (counter == N_THREADS) {
      active = true;
      cv.notify_one();
    }
  };

  for (unsigned i = 0; i < N_THREADS - 1; i++)
    threads[i].send_function(std::bind(lambda, i * segment, (i + 1) * segment), callback);
  threads.back().send_function(std::bind(lambda, (N_THREADS - 1) * segment, size), callback);

  std::unique_lock<std::mutex> lck(mtx); // wake up thread
  cv.wait(lck, [&]{return active;});

}

CallableThread::CallableThread() :
  t (&CallableThread::run, this)
{}

bool CallableThread::send_function(const std::function<void ()> func, const std::function<void ()> callback) {
  f = func;
  c = callback;
  active = true;
  cv.notify_one();
  return true;
}

void CallableThread::end_life() {
  alive = false;
  active = true;
  f = [&]() {};
  cv.notify_one();
}

void CallableThread::run() {
  while (alive) {
    std::unique_lock<std::mutex> lck(mtx); // wake up thread
    cv.wait(lck, [&]{return active;});
    f ();
    active = false;
    c ();
  }
}

const auto& instance = MultithreadManager::get_instance();
