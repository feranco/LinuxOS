#include <thread>
#include <iostream>

void sum (int upperLimit, int* result) {
  for (int i = 0; i <= upperLimit; ++i) {
    *result += i;
  }
}

int main (int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "specify an integer as command line argument\n";
    return -1;
  }

  int upperLimit = atoi (argv[1]);

  if (upperLimit < 0) {
    std::cerr << "Input integer shall be positive\n";
    return -1;
  }

  int result = 0;
  std::thread t{sum, upperLimit, &result};
  t.join();

  std::cout << "Sum is " << result << std::endl;
  
}
