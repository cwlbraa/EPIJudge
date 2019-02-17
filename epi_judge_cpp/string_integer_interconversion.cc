#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::string;
using std::cout;
using std::endl;

const std::unordered_map<char, int> ctoi = {
  {'0', 0},
  {'1', 1},
  {'2', 2},
  {'3', 3},
  {'4', 4},
  {'5', 5},
  {'6', 6},
  {'7', 7},
  {'8', 8},
  {'9', 9},
};

const std::array<char, 10> itoc = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

string IntToString(int x) {
  if (x == 0) {
    return "0";
  }
  if (x < 0) {
    return string{"-"}.append(IntToString(-1 * x));
  }
  string result = "";
  while (x > 0) {
    int digit = abs(x % 10);
    result.push_back(itoc[digit]);

    x /= 10;
  }
  std::reverse(result.begin(), result.end());
  return result;
}
int StringToInt(const string& s) {
  if (s.front() == '-') {
    return -1 * StringToInt(s.substr(1, s.size() - 1));
  }

  int result = 0;
  int i = 1;
  int exponent = s.size() - 1;
  for (const char& c : s) {
    result += ctoi.at(c) * pow(10, exponent);
    i++;
    exponent--;
  }
    // cout << result << endl;
  return result;
}
void Wrapper(int x, const string& s) {
  if (IntToString(x) != s) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
