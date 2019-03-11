#include <vector>
#include <map>
#include "test_framework/generic_test.h"
using std::vector;
using std::map;

vector<vector<int>> Permutations_(map<int, int>& counts) {
  vector<vector<int>> rez{};
  for (auto& p : counts) {
    if (p.second <= 0) continue;
    p.second--;
    auto smaller = Permutations_(counts);
    for (auto perm : smaller) {
      perm.push_back(p.first);
      rez.push_back(perm);
    }
    p.second++;
  }
  if (rez.empty()) {
    return {{}};
  }
  return rez;
}

vector<vector<int>> Permutations(vector<int> A) {
  map<int, int> counts{};
  for (int i : A) counts[i]++;
  return Permutations_(counts);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "permutations.cc", "permutations.tsv", &Permutations,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
