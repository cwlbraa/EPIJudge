#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>>& tree, int k) {
  if (!tree) return {};

  vector<int> rez = FindKLargestInBST(tree->right, k);
  if (rez.size() == k) return rez;

  rez.push_back(tree->data);
  if (rez.size() == k) return rez;

  auto left = FindKLargestInBST(tree->left, k - rez.size());
  rez.insert(rez.end(), left.begin(), left.end());
  return rez;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "k"};
  return GenericTestMain(args, "k_largest_values_in_bst.cc",
                         "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
