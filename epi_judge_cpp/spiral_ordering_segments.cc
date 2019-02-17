#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> MatrixInSpiralOrder(vector<vector<int>> square_matrix) {
  int tall = square_matrix.size();
  if (tall == 0) {
    return {};
  }

  vector<int>& head = square_matrix[0];
  if (tall == 1) {
    return head;
  }

  vector<vector<int>> remaining {};
  for (int i = 1; i < tall - 1; i++) {
    auto row = square_matrix[i];
    head.push_back(row.back());
    remaining.push_back(std::vector<int>{row.begin() + 1, row.end() - 1});
  }

  vector<int>& tail = square_matrix[tall - 1];
  head.insert(head.end(), tail.rbegin(), tail.rend());

  for (int i = 1; i < tall - 1; i++) {
    head.push_back((square_matrix.rbegin() + i)->front());
  }

  auto recurse = MatrixInSpiralOrder(remaining);
  head.insert(head.end(), recurse.begin(), recurse.end());

  return head;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "spiral_ordering_segments.cc",
                         "spiral_ordering_segments.tsv", &MatrixInSpiralOrder,
                         DefaultComparator{}, param_names);
}
