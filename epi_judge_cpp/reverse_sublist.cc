#include "list_node.h"
#include "test_framework/generic_test.h"

using PNode = shared_ptr<ListNode<int>>;

// reverse the WHOLE given list, returning the new head
PNode RevEach(PNode L) {
  PNode iter = L;
  PNode prev = nullptr;
  PNode next = iter->next;
  while (next != nullptr) {
    next = iter->next;
    iter->next = prev;
    prev = iter;
    iter = next;
  }

  return prev;
}

PNode ReverseSublist(PNode L, int start, int finish) {
  if (start == finish) {
    return L;
  }

  auto hed = make_shared<ListNode<int>>(0, L);

  int i = 1;
  PNode before = hed;
  while (i < start) {
      before = before->next;
      i++;
  }

  auto sublist_tail = before;
  while (i <= finish) {
      sublist_tail = sublist_tail->next;
      i++;
  }

  auto sublist_head = before->next;
  auto after = sublist_tail->next;
  sublist_tail->next = nullptr;
  before->next = RevEach(sublist_head);
  sublist_head->next = after;

  return hed->next;
}


int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
