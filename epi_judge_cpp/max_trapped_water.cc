#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int Max_(vector<int>::const_iterator b,
         vector<int>::const_iterator e) {
  if (b == e) {
    return 0;
  }
  int width = e - b;
  int size = 0;
  int recurse = 0;
  if (*b > *e) {
    size = *e * width;
    recurse = Max_(b, e - 1);
  } else {
    size = *b * width;
    recurse = Max_(b + 1, e);
  }
  return std::max(size, recurse);
}

int GetMaxTrappedWater(const vector<int>& heights) {
  return Max_(heights.begin(), heights.end() - 1);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
                         &GetMaxTrappedWater, DefaultComparator{}, param_names);
}
