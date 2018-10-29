#include <array>
#include <fstream>
#include <thread>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <bitset>

//multithread class to validate
//sudoku solutions. Error cases not managed.
class SudokuValidator {
public:
  void fillBoard(char* filename); 
  bool validateBoard(void);
private:
  void validateRow(int row, int threadId);
  void validateColumn(int col, int threadId);
  void validateBlock(int row, int col, int threadId);
  void printThreadState (int threadId, const std::string& state);
  static const int boardSize = 9;
  static const int threadNum = 27;
  std::array<std::array<int,boardSize>,boardSize> board{};
  std::bitset<threadNum> threadResult;  
};

void SudokuValidator::printThreadState (int threadId, const std::string& state) {
  std::stringstream message;
  message << "thread " << threadId << " " << state << "\n";
  std::cout << message.str();
}

void SudokuValidator::validateRow(int row, int threadId) {
  std::bitset<boardSize> isInRow;
  for (int i = 0; i < boardSize; ++i) {
    isInRow.set(board[row][i]-1);
  }

  if (isInRow.all()) threadResult.set(threadId);
}

void SudokuValidator::validateColumn(int col, int threadId) {
  std::bitset<boardSize> isInCol;
  for (int i = 0; i < boardSize; ++i) {
    isInCol.set(board[i][col]-1);
  }

  if (isInCol.all()) threadResult.set(threadId); 
}

void SudokuValidator::validateBlock(int row, int col, int threadId) {
  const int blockSize = 3;
  std::bitset<boardSize> isInBlock;
  for (int i = row; i < row + blockSize; ++i) {
    for (int j = col; j < col + blockSize; ++j) {
      isInBlock.set(board[i][j]-1);
    }
  }

  if (isInBlock.all()) threadResult.set(threadId);
}

//import solution to be validated
void SudokuValidator::fillBoard(char* filename) {
  std::ifstream file{filename};
  if (!file) throw std::runtime_error("Cannot open input file!");
  int row = 0, col = 0;

  for (int value; file >> value;) {
    board[row][col] = value;
    if(++col == boardSize) {
      row += 1;
      col = 0;
    }
  }
}

//validate the solution
bool SudokuValidator::validateBoard() {
  std::array<std::thread, threadNum> threads;
  int threadId = 0;

  //9 threads validating rows
  for (int i = 0; i < boardSize; ++i, ++threadId) {
    threads[threadId] = std::move(std::thread(&SudokuValidator::validateRow,this,i,threadId));
  }

  //9 threads validating columns
  for (int i = 0; i < boardSize; ++i, ++threadId) {
    threads[threadId] = std::move(std::thread(&SudokuValidator::validateColumn,this,i,threadId));
  }

  //9 threads validating 3x3 blocks
  for (int i = 0; i < boardSize; i+=3) {
    for (int j = 0; j < boardSize; j+=3) {
      threads[threadId] = std::move(std::thread(&SudokuValidator::validateBlock,this,i,j,threadId));
      ++threadId;
    }
  }

  for (auto& t : threads ) t.join();
  return threadResult.all();
}

int main (int argc, char** argv) {

  if (argc != 2) throw std::runtime_error("No input file specified!");

  SudokuValidator validator;
  validator.fillBoard(argv[1]);

  if (validator.validateBoard()) {
    std::cout << "The sudoku solution is valid!\n";
  }
  else {
    std::cout << "The sudoku solution is not valid!\n";
  }
}
