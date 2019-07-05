#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <thread>
#include <future>
#include <functional>
#include <vector>

#define N_THREADS 8

class MultithreadManager {
public:

  static MultithreadManager& get_instance () {
    static MultithreadManager instance;
    return instance;
  }

  void calculate_threaded (unsigned size, std::function<void (unsigned i)> f);

private:
  MultithreadManager () {}

  MultithreadManager (MultithreadManager const &) = delete;
  void operator= (MultithreadManager const &) = delete;
};

#endif // MULTITHREADMANAGER_H
