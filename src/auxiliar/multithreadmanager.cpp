#include "multithreadmanager.h"

void MultithreadManager::calculate_threaded(unsigned size,
                                            const std::function<void (unsigned)>& f) {
  auto lambda = [&](unsigned from, unsigned to) {
    for (unsigned i = from; i < to; i++)
      f(i);
  };

  double segment = double(size) / N_THREADS;
  unsigned counter = 0;

  active = false;

  std::mutex local_mtx;
  auto callback = [&] () {
    local_mtx.lock();
    counter++;
    unsigned c = counter;
    local_mtx.unlock ();

    if (c == N_THREADS) {
      active = true;
      cv.notify_one();
    }
  };

  for (unsigned i = 0; i < N_THREADS; i++)
    threads[i].send_function(std::bind(lambda, std::round(i * segment),
                                               std::round((i + 1) * segment)),
                                               callback);

  std::unique_lock<std::mutex> lck(mtx);
  cv.wait(lck, [&]{return active;});
  lck.unlock();
}

CallableThread::CallableThread() :
  t (&CallableThread::run, this)
{}

bool CallableThread::send_function(const std::function<void ()> func,
                                   const std::function<void ()> callback) {
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
    lck.unlock();
    f ();
    active = false;
    c ();
  }
}

const auto& instance = MultithreadManager::get_instance();
