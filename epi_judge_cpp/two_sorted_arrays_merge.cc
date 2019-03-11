#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void MergeTwoSortedArrays(vector<int>& A, int m, const vector<int>& B, int n) {
  int ai = m - 1;
  int bi = n - 1;
  int ti = m + n - 1;
  while (ai >= 0 || bi >= 0) {
    int av = std::numeric_limits<int>::min();
    int bv = std::numeric_limits<int>::min();
    if (ai >= 0) av = A[ai];
    if (bi >= 0) bv = B[bi];
    if (av >= bv) {
      A[ti--] = A[ai--];
    } else {
      A[ti--] = B[bi--];
    }
  }
  return;
}
vector<int> MergeTwoSortedArraysWrapper(vector<int> A, int m,
                                        const vector<int>& B, int n) {
  MergeTwoSortedArrays(A, m, B, n);
  return A;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "m", "B", "n"};
  return GenericTestMain(
      args, "two_sorted_arrays_merge.cc", "two_sorted_arrays_merge.tsv",
      &MergeTwoSortedArraysWrapper, DefaultComparator{}, param_names);
}
