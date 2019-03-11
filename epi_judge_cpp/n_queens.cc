#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include "test_framework/generic_test.h"
using std::vector;
using std::set;

bool IsValid(vector<int>& partial) {
  auto freshc = partial.size() - 1;
  auto freshr = partial[freshc];
  for (int i = 0; i < freshc; i++) {
    auto diff = std::abs(freshr - partial[i]);
    if (diff == freshc - i) {
      return false; // diagonal
    }
    if (diff == 0) {
      return false; // row
    }
  }

  return true;
}

void ProduceNQueensSolutions(int n, int c, vector<int>& partial, vector<vector<int>>& rez) {
  if (n == c) {
    rez.push_back(partial);
    return;
  }
  for (int r = 0; r < n; r++) {
    partial.resize(c+1);
    partial[c] = r;
    if (!IsValid(partial)) continue;
    ProduceNQueensSolutions(n, c+1, partial, rez);
  }
}

vector<vector<int>> NQueens(int n) {
  vector<vector<int>> rez{};
  vector<int> partial{};
  ProduceNQueensSolutions(n, 0, partial, rez);
  return rez;
}

bool Comp(vector<vector<int>>& a, vector<vector<int>>& b) {
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  return a == b;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                         param_names);
}
