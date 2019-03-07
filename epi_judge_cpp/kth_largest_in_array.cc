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

// The numbering starts from one, i.e., if A = [3, 1, -1, 2] then
// FindKthLargest(1, A) returns 3, FindKthLargest(2, A) returns 2,
// FindKthLargest(3, A) returns 1, and FindKthLargest(4, A) returns -1.
int FindKthLargest(int k, vector<int>* A_ptr) {
  k = k - 1; // why would you 1 index, you dick?
  vector<int> candidate{A_ptr->begin(), A_ptr->end()};
  int last_size = A_ptr->size();
  while(true) {
    vector<int> smaller{};
    vector<int> bigger{};
    int pivoti = rand() % candidate.size();
    int pivot = candidate[pivoti];
    std::cout << std::endl << candidate;
    std::cout << std::endl << pivot;
    for (auto i : candidate) {
      if (i == pivot) continue;
      if (i < pivot)
        smaller.push_back(i);
      } else {
        bigger.push_back(i);
      }
    }
    if (bigger.size() == k) {
      return pivot;
    }
    if (smaller.size() == last_size) {
      return pivot;
    }
    if (bigger.size() < k) {
      candidate = smaller;
      k = k - bigger.size();
    } else {
      candidate = bigger;
    }
  }
  return 0;
}
int FindKthLargestWrapper(int k, vector<int>& A) {
  return FindKthLargest(k, &A);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
