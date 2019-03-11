#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using std::unordered_map;

int FindNearestRepetition(const vector<string>& paragraph) {
  unordered_map<string, int> lpos;
  int rez = std::numeric_limits<int>::max();
  for (int i = 0; i < paragraph.size(); i++) {
    auto it = lpos.find(paragraph[i]);
    if (it != lpos.end()) {
      rez = std::min(rez, i - it->second);
    }
    lpos[paragraph[i]] = i;
  }
  return rez == std::numeric_limits<int>::max() ? -1 : rez;
}


int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"paragraph"};
  return GenericTestMain(args, "nearest_repeated_entries.cc",
                         "nearest_repeated_entries.tsv", &FindNearestRepetition,
                         DefaultComparator{}, param_names);
}
