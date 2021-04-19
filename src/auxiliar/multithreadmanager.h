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

  bool send_function (const std::function<void (void)> func,
                      const std::function<void (void)> callback);

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
