#include <deque>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>

using std::cout;
using std::cerr;
using std::deque;
using std::pair;
using std::string;
using std::stringstream;

//Class implementing an history feature for the shell
class History {
 public:
  History();
  ~History();
  string processCommand(const string& command);
  
 private:
  void update(const string& command);
  string lastCommand() const;
  
  static const int size = 10;
  int commandId;
  deque<pair<int,string>> commands;
};

History::History() : commandId(1) {

}

History::~History() {

}

//check if the command concerns the history feature
//and update the history with the latest command
string History::processCommand(const string& command) {
  stringstream ss{command};
  string firstToken;
  getline(ss, firstToken, ' ');

  //prompt the most recent commands
  if (firstToken == "history") {
    for (const auto& cmd : commands) {
      cout << cmd.first << " " << cmd.second << "\n";
    }
    return "";
  }

  //not an history command
  if (firstToken[0] != '!') {
    update(command);
    return command;
  }

  //execute the most recent command
  if (firstToken == "!!") {
    string cmd = lastCommand();
    cout << cmd << "\n";
    update(cmd);
    return cmd;
  }

  //try to execute one command in the history
  string idString(next(command.begin()), command.end());
  int idNumeric = stoi(idString);
  int lowerId = commands.begin()->first;
  cout << idNumeric << "\n";
  if (idNumeric >= lowerId && idNumeric < commandId) {
    string cmd = commands[idNumeric - lowerId].second;
    cout << cmd << "\n";
    update(cmd);
    return cmd;
  }

  cerr << "No such command in history.\n";
  return "";
}

//update the history with the last prompted command 
void History::update(const string& command) {
  commands.emplace_back(make_pair(commandId, command));
  if (commands.size() > size) commands.pop_front();
  commandId++;
}

//get the last prompted command
string History::lastCommand() const {
  return commands.back().second;
}

