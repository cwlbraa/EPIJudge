#include <vector>
#include <map>
#include "test_framework/generic_test.h"
using std::vector;

std::ostream& operator<<(std::ostream& o, vector<int> v) {
  o << "[";
  int c = 0;
  for (int i : v) {
   o << i << ":" << c++ << ", ";
  }
  o << "]";
  return o;
}

std::ostream& operator<<(std::ostream& o, vector<vector<int>> v) {
  for (auto i : v) {
   o << i << std::endl;
  }
  return o;
}

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int>& scores) {
  vector<vector<int>> ways{};
  for (int s = 0; s < scores.size(); s++) {
    ways.push_back({1});
    for (int i = 1; i <= final_score; i++) {
      ways[s].push_back(0);

      int prev = i - scores[s];
      if (prev >= 0)
        ways[s][i] += ways[s][prev];

      if (s != 0)
        ways[s][i] += ways[s - 1][i];
    }
  }
  // std::cout << ways << std::endl;
  return ways[scores.size() - 1][final_score];
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
