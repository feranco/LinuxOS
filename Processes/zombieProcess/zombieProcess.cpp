#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int main () {

  pid_t pid;
  pid = fork ();
  if (pid > 0) {
    // parent process will sleep for 30 seconds and exit, without a call to wait()
    std::cout << "parent process pid: " <<  getpid() << "\n";
    std::this_thread::sleep_for (std::chrono::seconds(30));
    //sleep(30); 
    exit(0);
  }
  else if (pid == 0) {
    // child process will exit immediately
    std::cout << "child process pid: " <<  getpid() << "\n";
    //fprintf(stderr,"child process - %d\n", getpid());
    exit(0);    
  }
  else if (pid == -1) {
    std::cout << "fork() call failed /n";
    exit (-1);
  }
  else {
    // this should not happen
    //fprintf(stderr, "unknown return value of %d from fork() call", child_pid);
    exit (-2);
  }
  return 0;

}
