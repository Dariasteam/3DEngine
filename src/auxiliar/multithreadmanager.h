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
#include <future>

#define N_THREADS 16

class CallableThread {
private:
  std::thread t;

  std::function<void(void)> empty_f = [](){};
  std::function<void (void)>* f = &empty_f;

  std::mutex mtx;
  std::condition_variable cv;

  bool active {false};
  bool alive  {true};

public:
  CallableThread ();

  bool send_function (std::function<void (void)>& func);

  bool is_active () { return active; }
  void end_life ();

private:
  void run ();
};

class MultithreadManager {
private:
  CallableThread threads[N_THREADS];
  std::mutex mtx;
  std::condition_variable cv;
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

  bool finished = false;

  std::mutex local_mtx;

  std::function<void(void)> functions [N_THREADS];

  for (unsigned i = 0; i < N_THREADS; i++) {
    functions[i] = [=, &f, &local_mtx, &counter, &finished]() {
      unsigned from = std::round(i * segment);
      unsigned to   = std::round((i + 1) * segment);

      auto it1 = begin + from;
      auto it2 = begin + to;

      while (it1 != it2) {
        f(*it1);
        it1++;
      }
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

private:

  MultithreadManager () {}

  MultithreadManager (MultithreadManager const &) = delete;
  void operator= (MultithreadManager const &) = delete;

  ~MultithreadManager () {
    for (auto& callable_thread : threads)
      callable_thread.end_life();
  }
};

#endif // MULTITHREADMANAGER_H
