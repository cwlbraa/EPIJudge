#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using std::unordered_map;

int Distance(const string& A, const string& B,
             int ae, int be, vector<vector<int>>& cache);

int Recurse(const string& A, const string& B,
             int ae, int be, vector<vector<int>>& cache) {
  if (A[ae] == B[be]) {
    return Distance(A, B, ae - 1, be - 1, cache);
  }

  return 1 + std::min({ Distance(A, B, ae, be - 1, cache) // insert into A
                      , Distance(A, B, ae - 1, be - 1, cache) // replace
                      , Distance(A, B, ae - 1, be, cache) // delete from A
                     });
}

int Distance(const string& A, const string& B,
             int ae, int be, vector<vector<int>>& cache) {
  if (ae < 0) return be + 1;
  if (be < 0) return ae + 1;
  if (cache[ae][be] == -1) {
    cache[ae][be] = Recurse(A, B, ae, be, cache);
  }

  return cache[ae][be];
}

int LevenshteinDistance(const string& A, const string& B) {
  vector<vector<int>> cache(A.size(), vector<int>(B.size(), -1));
  return Distance(A, B, A.size() - 1, B.size() - 1, cache);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
