#include <thread>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>

unsigned int min;
unsigned int max;
unsigned int avg;

int main(void) {

  auto minNumber = [](const std::vector<unsigned int>& numbers){
		     min = *std::min_element(numbers.begin(), numbers.end());};

  auto maxNumber = [](const std::vector<unsigned int>& numbers){
		     max = *std::max_element(numbers.begin(), numbers.end());};

  auto average = [](const std::vector<unsigned int>& numbers){
		   avg = std::accumulate(numbers.begin(), numbers.end(),0.0) / 2;};

  std::random_device rd;
  std::default_random_engine e{rd()};
  std::uniform_int_distribution<unsigned int> d{0, 1000};

  std::vector<unsigned int> numbers;
  for(int i = 0; i < 100; ++i){
    numbers.push_back(d(e));
  }

  std::thread tMin{minNumber, std::cref(numbers)};
  std::thread tMax{maxNumber, std::cref(numbers)};
  std::thread tAv{average, std::cref(numbers)};

  tMin.join();
  tMax.join();
  tAv.join();

  std::ofstream file{"stats.txt"};
  if(!file) std::runtime_error("cannot open output file");
  std::for_each(numbers.begin(), numbers.end(), [&](unsigned int n){ file << n << " ";});
  file << "\nmin: " << min;
  file << "\nmax: " << max;
  file << "\navg: " << avg;


}
