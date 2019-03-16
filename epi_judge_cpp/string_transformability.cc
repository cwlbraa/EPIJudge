#include <string>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_set;
using std::vector;
using Queue = std::deque<string>;

std::ostream& operator<<(std::ostream& o, Queue q) {
  o << "[";
  for (auto& i : q) o << i << ", ";
  o<< "]";
  return o;
}
// Uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string& s, const string& t) {
  std::vector<char> alphabet(26);
  std::iota(alphabet.begin(), alphabet.end(), 'a');

  int distance = 1;
  Queue current;
  current.push_back(s);
  unordered_set<string> visited{s};
  Queue next;

  while (!current.empty()) {
    string candidate = current.front(); current.pop_front();
    for (int l = 0; l < candidate.size(); l++) {
      char prev = candidate[l];
      for (char letter : alphabet) {
        candidate[l] = letter;
        if (candidate == t) return distance;
        if (D.find(candidate) != D.end()
            && visited.find(candidate) == visited.end()) {
          visited.insert(candidate);
          next.push_back(candidate);
        }
      }
      candidate[l] = prev;
    }

    if (current.empty() && !next.empty()) {
      current = next;
      next = {};
      distance++;
    }
  }

  return -1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"D", "s", "t"};
  return GenericTestMain(args, "string_transformability.cc",
                         "string_transformability.tsv", &TransformString,
                         DefaultComparator{}, param_names);
}
