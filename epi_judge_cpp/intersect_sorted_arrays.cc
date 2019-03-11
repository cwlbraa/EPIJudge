#include <vector>
#include "test_framework/generic_test.h"
#include <algorithm>
using std::vector;

vector<int> IntersectTwoSortedArrays(const vector<int>& A,
                                     const vector<int>& B) {
  vector<int> result{};
  int ap = 0; int bp = 0;
  while(ap < A.size() && bp < B.size()) {
    if (A[ap] == B[bp]) {
      auto intersecting = A[ap++]; bp++;
      if (!result.empty() && result.back() == intersecting) {
        continue;
      }
      result.push_back(intersecting);
    } else if (A[ap] < B[bp]) {
      ap++;
    } else if (A[ap] > B[bp]) {
      bp++;
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(
      args, "intersect_sorted_arrays.cc", "intersect_sorted_arrays.tsv",
      &IntersectTwoSortedArrays, DefaultComparator{}, param_names);
}
