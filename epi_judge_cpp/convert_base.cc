#include <string>
#include "test_framework/generic_test.h"
using std::string;
using std::cout;
using std::endl;

int intify(char c) {
  return isdigit(c) ? c - '0'
                    : c - 'A' + 10;
}

char charify(int i) {
  return i < 10 ? '0' + i
                : 'A' + (i - 10);
}

string ConstructFromInt(int x, int b2) {
  if (x == 0) {
    return "";
  }

  return ConstructFromInt(x / b2, b2) + charify(x % b2);
};

string ConvertBase(const string& num_as_string, int b1, int b2) {
  if (num_as_string.front() == '-') {
    return "-" + ConvertBase(num_as_string.substr(1, num_as_string.size() - 1), b1, b2);
  }

  int val = std::accumulate(num_as_string.begin(), num_as_string.end(), 0,
      [=](int running, char c) {
        return running * b1 + intify(c);
      }
  );

  if (val == 0) {
    return "0";
  }

  return ConstructFromInt(val, b2);
}


int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
