#include "multithreadmanager.h"

void MultithreadManager::calculate_threaded(unsigned size, std::function<void (unsigned)> f) {

  auto lambda = [&](unsigned from, unsigned to) {
    for (unsigned i = from; i < to; i++)
      f(i);
  };

  unsigned segment = size / N_THREADS;
  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS - 1; i++)
    promises[i] = std::async(lambda, i * segment, (i + 1) * segment);
  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segment, size);

  for (auto& promise : promises)
    promise.get();
}