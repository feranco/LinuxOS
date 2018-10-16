#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include "history.h"

#define MAX_ARGUMENTS 10

using std::cin;
using std::cout;
using std::getline;
using std::stringstream;
using std::string;


//class implementing a custom unix shell
class Shell {
 public:
  Shell (const string& s);
  void Run();
 private:
  int parseCommand(const string& command, char** cmd, char** args);
  string prompt; /* name of the shell */
 
};

Shell::Shell(const string& s) {
  prompt = s + ">";
};

void Shell::Run() {
  char *args[MAX_ARGUMENTS] = {(char*)nullptr}; /* command line arguments */
  char* cmd = nullptr;
  int should_run = 1; /* flag to determine when to exit program */
  History history;
  while (should_run) {
  
    cout << prompt;
    string command;
    getline(cin, command);
    command = history.processCommand(command);

    //command concerning only history feature
    if (command.empty()) continue;
    
    int nArgs = parseCommand(command, &cmd, args);

    //exit command
    if (nArgs < 0) break;

    //when last token is '&' the command executes concurrently with the shell
    bool isChildConcurrent = !strcmp(const_cast<char*>(args[nArgs-1]),"&");

    if(isChildConcurrent) {
      delete[] args[nArgs-1];
      args[nArgs-1] = nullptr;
    }

    pid_t pid;
    pid = fork();
    
    if (pid > 0) {
      //parent (the shell)
      if (!isChildConcurrent) {
	wait(nullptr);
      }
    }
    else if (pid == 0) {
      //child (the command)
      execv(cmd,args);
      std::cerr << "Execv not executed \n";
    }
  }
}

//parse the command prompted into the shell
int Shell::parseCommand(const string& command, char** cmd, char** args) {
  stringstream ss(command);
  string arg;
  string cmdPath = "/bin/";
  
  int nArgs = 0;
  while (getline(ss, arg, ' ')) {
    args[nArgs] = new char[arg.size()+1];
    std::strcpy (args[nArgs], arg.c_str());
    nArgs++;
  }

  string firstToken = args[0];
  cmdPath += firstToken;
  *cmd = new char[80];
  std::strcpy (*cmd, cmdPath.c_str());

  if (firstToken == "exit") nArgs = -1;

  return nArgs;
}
