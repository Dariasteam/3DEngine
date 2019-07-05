#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include "../../engine/point3d.h"

#include <thread>
#include <future>
#include <functional>

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
