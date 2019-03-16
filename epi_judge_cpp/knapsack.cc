#include <vector>
#include <algorithm>
#include <map>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
using std::map;

struct Item {
  int weight, value;
};

using iter = vector<Item>::const_iterator;
int Weights(iter current, iter e, int capacity, map<int, int>& cache) {
  if (current == e) return 0;
  auto ret = Weights(current + 1, e, capacity);
  if (current->weight <= capacity) {
    ret = std::max(ret, current->value + Weights(current + 1, e, capacity - current->weight));
  }
  return ret;
}

int OptimumSubjectToCapacity(const vector<Item>& items, int capacity) {
  map<int, int> cache{}; // cap to val
  return Weights(items.cbegin(), items.cend(), capacity, cache);
}

template <>
struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"items", "capacity"};
  return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
                         &OptimumSubjectToCapacity, DefaultComparator{},
                         param_names);
}
