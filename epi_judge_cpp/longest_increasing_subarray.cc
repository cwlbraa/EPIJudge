#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

struct Subarray {
  int start = 0, end = 0;
  bool operator<(const Subarray& other) const {
    return end - start < other.end - other.start;
  }
};

Subarray FindLongestIncreasingSubarray(const vector<int>& A) {
  Subarray longest = {0, 0};
  Subarray current = {0, 0};
  for (int i = 1; i < A.size(); i++) {
    if (A[i-1] < A[i]) {
      current.end = i;
      continue;
    }

    longest = std::max(current, longest);
    current = {i, i};
  }

  return std::max(current, longest);
}

int FindLongestIncreasingSubarrayWrapper(const vector<int>& A) {
  Subarray result = FindLongestIncreasingSubarray(A);
  return result.end - result.start + 1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_increasing_subarray.cc", "longest_increasing_subarray.tsv",
      &FindLongestIncreasingSubarrayWrapper, DefaultComparator{}, param_names);
}
