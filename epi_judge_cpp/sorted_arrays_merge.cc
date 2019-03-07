#include <vector>
#include <queue>
#include "test_framework/generic_test.h"

using std::vector;
using std::priority_queue;

struct QItem {
  QItem(int val, int oai) : val(val), oai(oai) {}

  int val;
  int oai; // origin array index
  bool operator>(QItem other) const {
    return val > other.val;
  }
};


vector<int> MergeSortedArrays_(const vector<vector<int>>& sorted_arrays) {
  priority_queue<QItem, vector<QItem>, std::greater<QItem>> q{};
  for (int i = 0; i < sorted_arrays.size(); i++) {
    q.emplace(sorted_arrays[i][0], i);
  }

  vector<int> indices(sorted_arrays.size(), 1);
  while(!q.empty()) {
  }

  return {};
}
vector<int> MergeSortedArrays(const vector<vector<int>>& sorted_arrays) {
  priority_queue<int, vector<int>, std::greater<int>> q{};
  for (int i = 0; i < sorted_arrays.size(); i++) {
    for (int j = 0; j < sorted_arrays[i].size(); j++) {
      q.push(sorted_arrays[i][j]);
    }
  }
  vector<int> rez{};
  while(!q.empty()) {
    auto val = q.top(); q.pop();
    rez.push_back(val);
  }
  return rez;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
