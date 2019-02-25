#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;

void ReverseWords(string* s) {
  std::reverse(s->begin(), s->end());
  auto wordstart = s->begin();
  for (auto iter = s->begin(); iter != s->end(); iter++) {
    if (!isalnum(*iter)) {
      std::reverse(wordstart, iter);
      wordstart = iter + 1;
    }
  }

  if (wordstart < s->end()) {
      std::reverse(wordstart, s->end());
  }
}

string ReverseWordsWrapper(TimedExecutor& executor, string s) {
  string s_copy = s;

  executor.Run([&] { ReverseWords(&s_copy); });

  return s_copy;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "s"};
  return GenericTestMain(args, "reverse_words.cc", "reverse_words.tsv",
                         &ReverseWordsWrapper, DefaultComparator{},
                         param_names);
}
