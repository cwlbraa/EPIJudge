#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

using iter = vector<int>::const_iterator;



int FindMaximumSubarray(const vector<int>& A) {
  int max_seen = 0; int max_here = 0;
  for (int a : A) {
    max_here = std::max(a, max_here + a);
    max_seen = std::max(max_seen, max_here);
  }
  return max_seen;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_sum_subarray.cc", "max_sum_subarray.tsv",
                         &FindMaximumSubarray, DefaultComparator{},
                         param_names);
}
