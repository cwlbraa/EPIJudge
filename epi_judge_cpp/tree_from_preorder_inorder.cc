#include <algorithm>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
using std::vector;

unique_ptr<BinaryTreeNode<int>> BinaryTreeFromPreorderInorder(
    const vector<int>& preorder, const vector<int>& inorder) {
  if (preorder.size() == 0) {
    return nullptr;
  }
  auto xi = std::find(inorder.begin(), inorder.end(), preorder[0]);
  auto size = xi - inorder.begin();

  vector<int> prel{preorder.begin() + 1, preorder.begin() + size + 1};
  vector<int> inl{inorder.begin(), inorder.begin() + size};
  vector<int> prer{preorder.begin() + size + 1, preorder.end()};
  vector<int> inr{xi + 1, inorder.end()};

  return std::make_unique<BinaryTreeNode<int>>(
    preorder[0],
    BinaryTreeFromPreorderInorder(prel, inl),
    BinaryTreeFromPreorderInorder(prer, inr));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder", "inorder"};
  return GenericTestMain(
      args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
      &BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
