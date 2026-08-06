#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
std::mutex mtx;

// How many times the threads increment the shared counter
int Calculator(int32_t target, int t) { return static_cast<int>(target / t); }

// What the threads do
void ExecuteJob(int32_t& sharedCounter, const int n) {
  for (int i = 0; i < n; ++i) {
    std::lock_guard<std::mutex> lock(mtx);
    if (sharedCounter % 2 == 0) {
      sharedCounter++;

    } else if (sharedCounter >= 1 && sharedCounter % 2 == 1) {
      sharedCounter--;
    }
  }
}
// Spawn Threads
template <typename callback>
std::vector<std::thread> ThreadsSpawner(int numberofThreads, callback task) {
  std::vector<std::thread> vectorOfThreads;
  vectorOfThreads.reserve(numberofThreads);
  for (int i = 0; i < numberofThreads; ++i) {
    vectorOfThreads.emplace_back(task);
  }

  return vectorOfThreads;
}

int main() {
  const int NUMBEROFITERATIONS = 10;
  int NumberOfThreads = 4;
  int target = 10000000;
  int maxPerThread{Calculator(target, NumberOfThreads)};
  int32_t sharedCounter{0};
  size_t accumulator{0};

  for (int32_t i = 0; i < NUMBEROFITERATIONS; ++i) {
    sharedCounter = 0;
    // Time Started
    const auto timestarted{std::chrono::steady_clock::now()};
    // TO DO: refactor to use thread pools to avoid creating and deleting threads at every iteration
    auto threadSpawned = ThreadsSpawner(NumberOfThreads, [&](){ExecuteJob(sharedCounter,Calculator(target,NumberOfThreads));});
    for (auto& mythread : threadSpawned) {
      if (mythread.joinable()) {
        mythread.join();
      }
    }

    // Time finished
    const auto timefinished{std::chrono::steady_clock::now()};

    // accumulator holds the final value of sharedcounter
    accumulator += sharedCounter;
    const std::chrono::duration<double> timetaken{timefinished - timestarted};
    std::cout << "Iteration " << i << " took " << timetaken.count()
              << std::endl;
  }

  std::cout << "Sharecounter is now: " << sharedCounter << std::endl;
  std::cout << "The average shared counter value over " << NUMBEROFITERATIONS
            << " iterations is " << accumulator / NUMBEROFITERATIONS
            << std::endl;

  return 0;
}