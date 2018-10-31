#include <thread>
#include <random>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <algorithm>

int main (int argc, char**argv) {
  
  if (argc != 2) throw std::runtime_error("Specify the size of the array to be sorted");
  int size = atoi(argv[1]), m = size/2;

  std::vector<int> sequence(size);
  std::random_device rd;
  std::uniform_int_distribution<int> d{-1000, 1000};
  std::default_random_engine e{rd()};
  for (int i = 0; i < size; ++i) sequence[i] = d(e);

  //start: index first element
  //end: index last element
  auto sortingThread = [&](int start, int end){
			 auto startIt = sequence.begin();
			 auto endIt = sequence.begin();
			 std::advance(startIt, start);
			 std::advance(endIt, end+1);  //sort require next(end) 
			 std::sort(startIt, endIt);
		       };
  //start: index first element of forst half
  //m: index first element of second half
  //end: index last element of second half
  auto mergingThread = [&](int start, int m, int end) {
			 std::vector<int> tmp(end-start+1);
			 int i = start, j = m, k = 0;
			 
			 while (i < m && j <= end) {
			   if (sequence[j] < sequence[i]) {
			     tmp[k++] = sequence[j++];
			   }
			   else {
			     tmp[k++] = sequence[i++];
			   }
			 }

			 while (i < m) tmp[k++] = sequence[i++];
			 while (j <= end) tmp[k++] = sequence[j++];

			 sequence.assign(tmp.begin(), tmp.end());
		       };

  std::thread t1{sortingThread, 0, m-1}, t2{sortingThread, m, size-1};
  t1.join();
  t2.join();
  
  std::thread t3{mergingThread, 0, m, size-1};
  t3.join();
  
  std::for_each(sequence.begin(), sequence.end(), [](int x){
						    std::cout << x << " ";});
  
}

