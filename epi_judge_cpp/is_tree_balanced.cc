#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <cmath>
#include <algorithm>

using SNode = std::shared_ptr<BinaryTreeNode<int>>;

struct Ret {
  int depth;
  bool balanced;
};

Ret Depth(const unique_ptr<BinaryTreeNode<int>>& tree) {
  if (tree == nullptr) {
    return Ret { 0, true };
  }
  auto l = Depth(tree->left);
  auto r = Depth(tree->right);
  auto balanced = std::abs(l.depth - r.depth) < 2;
  return Ret{ 1 + std::max(l.depth, r.depth), balanced && l.balanced && r.balanced };
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>>& tree) {
  Ret l = Depth(tree->left);
  Ret r = Depth(tree->right);
  return (std::abs(l.depth - r.depth) < 2) && l.balanced && r.balanced;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
