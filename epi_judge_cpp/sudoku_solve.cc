#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::begin;
using std::end;
using std::vector;

bool ValidToAdd(int x, int y, int val, vector<vector<int>>* partial_assignment) {
  int tall = partial_assignment->at(0).size();
  int wide = partial_assignment->size();
  // column
  for (int j = 0; j < tall; j++) {
    if (j == y) continue;
    if (val == partial_assignment->at(x)[j]) return false;
  }
  // row
  for (int i = 0; i < wide; i++) {
    if (i == x) continue;
    if (val == partial_assignment->at(i)[y]) return false;
  }
  // box
  int boxx = x / 3;
  int boxy = y / 3;
  for (int i = 3 * boxx; i < 3 * (boxx + 1); i++) {
    for (int j = 3 * boxy; j < 3 * (boxy + 1); j++) {
      if (i == x && j == y) continue;
      if (val == partial_assignment->at(i)[j]) return false;
    }
  }
  return true;
}

vector<int> dig = {1,2,3,4,5,6,7,8,9};
bool SolveSudoku(vector<vector<int>>* partial_assignment,
                 int i, int j) {
  int wide = partial_assignment->size();
  int tall = partial_assignment->at(0).size();
  if (j == tall) {
    j = 0;
    if (++i == wide) return true;
  }

  if (partial_assignment->at(i)[j] > 0) return SolveSudoku(partial_assignment, i, j+1);

  for (auto d : dig) {
    if (ValidToAdd(i, j, d, partial_assignment)) {
      partial_assignment->at(i)[j] = d;
      if (SolveSudoku(partial_assignment, i, j+1)) return true;
    }
  }
  // if it's unsolved, the previous call on the stack
  // is gonna try a different assignement, so reset this one
  partial_assignment->at(i)[j] = 0;

  return false;
}

std::ostream& operator<<(std::ostream& o, vector<vector<int>> v) {
  o << std::endl;
  for (int j = 0; j < v.at(0).size(); j++) {
    for (int i = 0; i < v.size(); i++) {
      o << v[i][j] << " ";
    }
    o << std::endl;
  }
  o << std::endl;

  return o;
}

bool SolveSudoku(vector<vector<int>>* partial_assignment) {
  auto rez = SolveSudoku(partial_assignment, 0, 0);
  std::cout << *partial_assignment << std::endl;
  return rez;
}

vector<int> GatherColumn(const vector<vector<int>>& data, size_t i) {
  vector<int> result;
  for (auto& row : data) {
    result.push_back(row[i]);
  }
  return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>>& data,
                              size_t block_size, size_t n) {
  vector<int> result;
  size_t block_x = n % block_size;
  size_t block_y = n / block_size;
  for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
    for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
      result.push_back(data[i][j]);
    }
  }

  return result;
}

void AssertUniqueSeq(const vector<int>& seq) {
  vector<bool> seen(seq.size(), false);
  for (auto& x : seq) {
    if (x == 0) {
      throw TestFailure("Cell left uninitialized");
    }
    if (x < 0 || x > seq.size()) {
      throw TestFailure("Cell value out of range");
    }
    if (seen[x - 1]) {
      throw TestFailure("Duplicate value in section");
    }
    seen[x - 1] = true;
  }
}

void SolveSudokuWrapper(TimedExecutor& executor,
                        const vector<vector<int>>& partial_assignment) {
  vector<vector<int>> solved = partial_assignment;

  executor.Run([&] { SolveSudoku(&solved); });

  if (!std::equal(begin(partial_assignment), end(partial_assignment),
                  begin(solved), end(solved), [](auto br, auto cr) {
                    return std::equal(begin(br), end(br), begin(cr), end(cr),
                                      [](int bcell, int ccell) {
                                        return bcell == 0 || bcell == ccell;
                                      });
                  }))
    throw TestFailure("Initial cell assignment has been changed");

  auto block_size = static_cast<size_t>(sqrt(solved.size()));

  for (size_t i = 0; i < solved.size(); i++) {
    AssertUniqueSeq(solved[i]);
    AssertUniqueSeq(GatherColumn(solved, i));
    AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "partial_assignment"};
  return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                         &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
