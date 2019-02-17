#include "test_framework/generic_test.h"
#include "math.h"

double Power(double x, int y) {
  if (y == 0) {
    return 1;
  }

  double factor = x;
  int next = y - 1;

  if (y < 0) {
    factor = 1.0 / x;
    next = y + 1;
  }

  if (y % 2 != 0) {
    return factor * Power(x, next);
  }

  auto half = Power(x, y / 2);
  return half * half;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "power_x_y.cc", "power_x_y.tsv", &Power,
                         DefaultComparator{}, param_names);
}
