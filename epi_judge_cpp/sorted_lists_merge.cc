#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> MergeTwoSortedLists(shared_ptr<ListNode<int>> L1,
                                              shared_ptr<ListNode<int>> L2) {
  if (!L1) {
    return L2;
  }
  if (!L2) {
    return L1;
  }

  if (L1->data < L2->data) {
    L1->next = MergeTwoSortedLists(L1->next, L2);
    return L1;
  }

  L2->next = MergeTwoSortedLists(L1, L2->next);
  return L2;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L1", "L2"};
  return GenericTestMain(args, "sorted_lists_merge.cc",
                         "sorted_lists_merge.tsv", &MergeTwoSortedLists,
                         DefaultComparator{}, param_names);
}
