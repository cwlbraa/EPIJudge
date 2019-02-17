#include <vector>
#include <limits>
#include "test_framework/generic_test.h"

using std::vector;

double BuyAndSellStockOnce(const vector<double>& prices) {

  vector<double> a(prices.size());
  double running = std::numeric_limits<double>::max();

  double profit = 0.0;
  for (auto &item : prices) {
    running = std::min(running, item);
    profit = std::max(profit, item - running);
  }

  return profit;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock.cc",
                         "buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
                         DefaultComparator{}, param_names);
}
