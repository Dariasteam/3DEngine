#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <thread>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <future>

#define N_THREADS 8

class CallableThread {
private:
  std::thread t;
  std::function<void (void)> f;
  std::function<void (void)> c;

  std::mutex mtx;
  std::condition_variable cv;
  bool active = false;
  bool alive = true;

public:
  CallableThread () :
    t (&CallableThread::run, this)
  {}

  bool send_function (const std::function<void (void)> func,
                      const std::function<void (void)> callback) {
    f = func;
    c = callback;
    active = true;
    cv.notify_one();
    return true;
  }

  void start () {
    t.detach();
  }

  bool is_active () { return active; }

  void end_life () {
    alive = false;
    active = true;
    f = [&]() {};
    cv.notify_one();
  }

private:
  void run () {
    while (alive) {
      std::unique_lock<std::mutex> lck(mtx); // wake up thread
      cv.wait(lck, [&]{return active;});
      f ();
      active = false;
      c ();
    }
  }
};

class MultithreadManager {
private:
  std::vector<CallableThread> threads;
public:

  static MultithreadManager& get_instance () {
    static MultithreadManager instance;
    return instance;
  }

  void calculate_threaded (unsigned size, std::function<void (unsigned i)> f);

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
