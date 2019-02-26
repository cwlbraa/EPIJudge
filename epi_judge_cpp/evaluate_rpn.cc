#include <string>
#include "test_framework/generic_test.h"
#include <stack>
#include <set>
#include <iostream>
using std::string;

using arithmetic = std::function<int(int,int)>;

const std::map<string, arithmetic> ops{
  {"+", [](int x, int y) { return x + y; }},
  {"*", [](int x, int y) { return x * y; }},
  {"-", [](int x, int y) { return x - y; }},
  {"/", [](int x, int y) { return x / y; }},
};

int E_(std::vector<string> expression) {
  std::stack<int> p;
  for (string& s : expression) {
    auto oppair = ops.find(s);
    int val;
    if (oppair == ops.end()) {
      val = stoi(s);
    } else {
      auto r = p.top();
      p.pop();
      auto l = p.top();
      p.pop();
      auto f = oppair->second;
      val = f(l, r);
    }
    p.push(val);
  }
  return p.top();
}

int Evaluate(const string& expression) {
  std::vector<string> split{};
  std::stringstream ss{expression};
  while (ss.good()) {
    string s;
    getline(ss, s, ',');
    split.push_back(s);
  }
  return E_(split);
}


int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"expression"};
  return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
                         DefaultComparator{}, param_names);
}
