#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <string>
#include <sstream>

#include "pidManager.h"

void printThreadState (int pid, const std::string& state) {
  std::stringstream message;
  message << "thread " << pid << " " << state << "\n";
  std::cout << message.str();
}


void testPidManager (PidManager& pm, int waitSeconds) {
  
  unsigned int pid = pm.allocatePid();  
  printThreadState(pid, "started");
  
  std::this_thread::sleep_for(std::chrono::milliseconds(waitSeconds));
  
  pm.releasePid(pid);
  printThreadState(pid, "terminated");
  
}

int main(int argc, char** argv) {

  int threadNumber = 100;
  if (argc > 1) threadNumber  = atoi(argv[1]);

  PidManager pm;
  std::random_device rd{};
  std::default_random_engine e{rd()};
  std::uniform_int_distribution<unsigned int> d{1,61};
  
  std::vector<std::thread> threads;
  for (int i = 0; i < threadNumber; ++i) {
    //threads.push_back(std::move(std::thread(testPidManager/*, pm, e*/)));
    threads.emplace_back(testPidManager, std::ref(pm), d(e));
  }
  
  for (int i = 0; i < threadNumber; ++i) {
    threads[i].join();
  }
  
}
