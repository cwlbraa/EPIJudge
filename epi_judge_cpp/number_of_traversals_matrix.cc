#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int NumWays(int n, int m, int ni, int mi, vector<vector<int>>& cache) {
  if (n == ni + 1) return 1;
  if (m == mi + 1) return 1;
  if (cache[ni][mi] == -1) {
    cache[ni][mi] = NumWays(n, m, ni + 1, mi, cache)
                  + NumWays(n, m, ni, mi + 1, cache);
  }

  return cache[ni][mi];
}

int NumberOfWays(int n, int m) {
  vector<vector<int>> cache(n, vector<int>(m, -1));
  return NumWays(n, m, 0, 0, cache);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "m"};
  return GenericTestMain(args, "number_of_traversals_matrix.cc",
                         "number_of_traversals_matrix.tsv", &NumberOfWays,
                         DefaultComparator{}, param_names);
}
