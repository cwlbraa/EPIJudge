#include "test_framework/generic_test.h"

const unsigned int ones = -1;
const unsigned int mask = ones - 1;

short CountBits(unsigned int x) {
  unsigned int res = 0;
  while (x != 0)
  {
    res += x & 1;
    x = x >> 1;
  }
  return res;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "count_bits.cc", "count_bits.tsv", &CountBits,
                         DefaultComparator{}, param_names);
}
