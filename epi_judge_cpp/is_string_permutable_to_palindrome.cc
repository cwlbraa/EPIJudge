#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_map;

bool CanFormPalindrome(const string& s) {
  unordered_map<char, int> counts;
  for (auto c : s) counts[c]++;

  bool found_odd = false;
  for (auto p : counts) {
    if (p.second % 2 != 0) {
      if (found_odd) {
        return false;
      }
      found_odd = true;
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_permutable_to_palindrome.cc",
                         "is_string_permutable_to_palindrome.tsv",
                         &CanFormPalindrome, DefaultComparator{}, param_names);
}
