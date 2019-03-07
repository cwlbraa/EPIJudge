#include <vector>
#include <queue>
#include "test_framework/generic_test.h"

using std::vector;
using std::priority_queue;
using Q = priority_queue<int, vector<int>, std::greater<int>>;
vector<int> SortApproximatelySortedData(
    vector<int>::const_iterator sequence_begin,
    const vector<int>::const_iterator& sequence_end, int k) {

   Q q{sequence_begin, sequence_begin + k};

   vector<int> rez{};
   for (auto iter = sequence_begin + k; iter != sequence_end; iter++) {
     q.push(*iter);
     rez.push_back(q.top()); q.pop();
   }

   while(!q.empty()) {
     rez.push_back(q.top()); q.pop();
   }


  return rez;
}
vector<int> SortApproximatelySortedDataWrapper(const vector<int>& sequence,
                                               int k) {
  return SortApproximatelySortedData(cbegin(sequence), cend(sequence), k);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence", "k"};
  return GenericTestMain(
      args, "sort_almost_sorted_array.cc", "sort_almost_sorted_array.tsv",
      &SortApproximatelySortedDataWrapper, DefaultComparator{}, param_names);
}
