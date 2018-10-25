#include <bitset>

class PidManager {
public:
  int allocatePid (void);
  void releasePid (unsigned int pid);
private:
  static const unsigned int minPid = 300;
  static const unsigned int maxPid = 5000;
  static const unsigned int pidRangeSize = maxPid - minPid;
  std::bitset<pidRangeSize> pidState;
};

int PidManager::allocatePid (void) {
  for (unsigned int i = 0; i < pidRangeSize; ++i) {
    if (!pidState.test(i)) {
      pidState.set(i);
      return i + minPid;
    }
  }
  return -1;
}

void PidManager::releasePid (unsigned int pid) {
  pidState.reset(pid-minPid);
}

