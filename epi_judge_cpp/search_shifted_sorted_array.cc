#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

std::ostream& operator<<(std::ostream& os, const vector<int>& dt) {
  os << "[";
  for (int i : dt) {
    os << i << ", ";
  }
  os << "]";
  return os;
}


int SearchSmallest(const vector<int>& A) {
  int left = 0;
  int right = A.size() - 1;
  while (left < right) {
    if (right - left == 1) {
      return A[left] < A[right] ? left : right;
    }
    int mid = left + (right - left) / 2;
    if (A[mid] <= A[right]) {
      right = mid;
      continue;
    }
    if (A[mid] >= A[left]) {
      left = mid;
      continue;
    }
  }

  return left;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "search_shifted_sorted_array.cc",
                         "search_shifted_sorted_array.tsv", &SearchSmallest,
                         DefaultComparator{}, param_names);
}
