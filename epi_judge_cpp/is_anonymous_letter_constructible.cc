#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_map;

bool IsLetterConstructibleFromMagazine(const string& letter_text,
                                       const string& magazine_text) {
  unordered_map<char, int> lc{};
  unordered_map<char, int> mc{};
  for (auto c : letter_text) lc[c]++;
  for (auto c : magazine_text) mc[c]++;
  for (auto p : lc) {
    if (mc[p.first] < p.second) return false;
  }


  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"letter_text", "magazine_text"};
  return GenericTestMain(args, "is_anonymous_letter_constructible.cc",
                         "is_anonymous_letter_constructible.tsv",
                         &IsLetterConstructibleFromMagazine,
                         DefaultComparator{}, param_names);
}
