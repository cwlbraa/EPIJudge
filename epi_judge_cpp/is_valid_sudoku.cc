#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
// Check if a partially filled matrix has any conflicts.
bool IsValidSudoku(const vector<vector<int>>& partial_assignment) {
  std::array<std::array<bool, 10>, 9> columns{};
  std::array<std::array<bool, 10>, 9> rows{};
  std::array<std::array<bool, 10>, 9> squares{};

  int j = 0;
  for (auto row : partial_assignment) {
    int i = 0;
    for (int& item : row) {
      if (item < 1) {
        i++;
        continue;
      }

      int sq = i / 3 + (j / 3) * 3;
      if (columns[i][item] || rows[j][item] || squares[sq][item]) {
        return false;
      }

      columns[i][item] = true;
      rows[j][item] = true;
      squares[sq][item] = true;

      i++;
    }
    j++;
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"partial_assignment"};
  return GenericTestMain(args, "is_valid_sudoku.cc", "is_valid_sudoku.tsv",
                         &IsValidSudoku, DefaultComparator{}, param_names);
}
