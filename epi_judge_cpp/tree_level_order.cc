#include <memory>
#include <vector>
#include <list>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;
using std::list;

std::ostream& operator<<(std::ostream& os, const list<int>& dt) {
  os << "[";
  for (auto i : dt) os << i << ", ";
  os << "]";
  return os;
}

using t = const unique_ptr<BinaryTreeNode<int>>;
list<int> LevelN(t& tree, int n) {
  if (tree == nullptr) {
    return {};
  }
  if (n == 0) {
    return {tree->data};
  }
  auto rez = LevelN(tree->left, n - 1);
  rez.splice(rez.end(), LevelN(tree->right, n - 1));
  return rez;
}

vector<vector<int>> BinaryTreeDepthOrder(t& tree) {
  vector<vector<int>> rez {};
  auto level = LevelN(tree, 0);
  int i = 1;
  while (!level.empty()) {
    rez.push_back(vector<int>{level.begin(), level.end()});
    level = LevelN(tree, i++);
  }
  return rez;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_level_order.cc", "tree_level_order.tsv",
                         &BinaryTreeDepthOrder, DefaultComparator{},
                         param_names);
}
