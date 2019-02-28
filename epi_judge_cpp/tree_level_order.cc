#include <memory>
#include <vector>
#include <list>
#include <queue>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::list;
using std::queue;

std::ostream& operator<<(std::ostream& os, const list<int>& dt) {
  os << "[";
  for (auto i : dt) os << i << ", ";
  os << "]";
  return os;
}

using t = unique_ptr<BinaryTreeNode<int>>;
using ts = BinaryTreeNode<int>*;
vector<vector<int>> BinaryTreeDepthOrder(const t& tree) {
  vector<vector<int>> rez {};
  queue<ts> current_depth = {};
  current_depth.push(tree.get());
  queue<ts> next_depth = {};
  vector<int> running{};
  while (!current_depth.empty()) {
    auto& current = current_depth.front();
    current_depth.pop();
    if (current != nullptr) {
      running.push_back(current->data);
      next_depth.push(current->left.get());
      next_depth.push(current->right.get());
    }
    if (current_depth.empty()) {
      current_depth = next_depth;
      next_depth = {};
      if (!running.empty()) rez.push_back(running);
      running = {};
    }
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
