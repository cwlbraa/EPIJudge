#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

using Node = const unique_ptr<BinaryTreeNode<int>>&;
bool Matches(Node l, Node r) {
  if (!l && !r) {
    return true;
  }
  if (!l || !r) {
    return false;
  }
  if (l->data != r->data) {
    return false;
  }
  if (!Matches(l->left, r->right)) {
    return false;
  }
  if (!Matches(l->right, r->left)) {
    return false;
  }
  return true;
}

bool IsSymmetric(const unique_ptr<BinaryTreeNode<int>>& tree) {
  return Matches(tree->left, tree->right);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_symmetric.cc", "is_tree_symmetric.tsv",
                         &IsSymmetric, DefaultComparator{}, param_names);
}
