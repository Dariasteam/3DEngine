#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <thread>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <future>
#include <cmath>

#define N_THREADS 16

class CallableThread {
private:
  std::thread t;
  std::function<void (void)> f;
  std::function<void (void)> c;

  std::mutex mtx;
  std::condition_variable cv;
  bool active {false};
  bool alive  {true};

public:
  CallableThread ();

  bool send_function (const std::function<void (void)>& func,
                      const std::function<void (void)>& callback);

  void start () {
    t.detach();
  }

  bool is_active () { return active; }
  void end_life ();

private:
  void run ();
};

class MultithreadManager {
private:
  std::vector<CallableThread> threads;
public:

  static MultithreadManager& get_instance () {
    static MultithreadManager instance;
    return instance;
  }

  void calculate_threaded (unsigned size, const std::function<void (unsigned i)>& f);

  template <typename iterable, typename N>
  void calculate_threaded (iterable& collection,
                           const std::function<void (N& i)>& f) {

  auto begin     = collection.begin();
  auto size = collection.size();

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

  for (unsigned i = 0; i < N_THREADS; i++) {
    threads[i].send_function([=, &f]() {
      unsigned from = std::round(i * segment);
      unsigned to   = std::round((i + 1) * segment);

      auto it1 = begin + from;
      auto it2 = begin + to;

      while (it1 != it2) {
        f(*it1);
        it1++;
      }
    }, callback);
  }

  std::unique_lock<std::mutex> lck(mtx);
  cv.wait(lck, [&]{return active;});
  lck.unlock();
}

private:
  std::mutex mtx;
  std::condition_variable cv;
  bool active = false;

  void thread_function ();

  MultithreadManager () :
    threads (N_THREADS)
  {
    for (auto& callable_thread : threads)
      callable_thread.start();
  }

  MultithreadManager (MultithreadManager const &) = delete;
  void operator= (MultithreadManager const &) = delete;

  ~MultithreadManager () {
    for (auto& callable_thread : threads)
      callable_thread.end_life();
  }
};

#endif // MULTITHREADMANAGER_H
