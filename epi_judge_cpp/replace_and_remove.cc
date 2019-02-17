#include <iterator>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

// int ReplaceAndRemove(int size, char s[]) {
//   std::vector<char> scratch = {};

//   for (int i = 0; i < size; i++) {
//     if (s[i] == 'b') {
//       continue;
//     }
//     if (s[i] == 'a') {
//       scratch.push_back('d');
//       scratch.push_back('d');
//       continue;
//     }
//     scratch.push_back(s[i]);
//   }

//   for (int i = 0; i < scratch.size(); i++) {
//     s[i] = scratch[i];
//   }

//   return scratch.size();
// }

int fixchar(int target, char letter, char s[]) {
    if (letter == 'b') {
      return 0;
    }
    if (letter == 'a') {
      s[target] = 'd';
      s[target + 1] = 'd';
      return 2;
    }
    s[target] = letter;
    return 1;
}

int ReplaceAndRemove(int size, char s[]) {
  if (size == 0) {
    return 0;
  }

  char tail = s[size - 1];
  int fixedlen = ReplaceAndRemove(size - 1, s);
  return fixedlen + fixchar(fixedlen, tail, s);
}


vector<string> ReplaceAndRemoveWrapper(TimedExecutor& executor, int size,
                                       const vector<string>& s) {
  std::vector<char> s_copy(s.size(), '\0');
  for (int i = 0; i < s.size(); ++i) {
    if (!s[i].empty()) {
      s_copy[i] = s[i][0];
    }
  }

  int res_size =
      executor.Run([&] { return ReplaceAndRemove(size, s_copy.data()); });

  vector<string> result;
  for (int i = 0; i < res_size; ++i) {
    result.emplace_back(string(1, s_copy[i]));
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "size", "s"};
  return GenericTestMain(args, "replace_and_remove.cc",
                         "replace_and_remove.tsv", &ReplaceAndRemoveWrapper,
                         DefaultComparator{}, param_names);
}
