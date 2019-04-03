#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <unordered_set>

using std::shared_ptr;

shared_ptr<ListNode<int>> Advance(shared_ptr<ListNode<int>> l0, int iters) {
  if (!iters) return l0;
  if (!l0) return nullptr;
  return Advance(l0->next, iters - 1);
}

int Length(shared_ptr<ListNode<int>> l0) {
  if (!l0) return 0;
  return 1 + Length(l0->next);
}

shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {

  int l0len = Length(l0);
  int l1len = Length(l1);

  if (l0len > l1len) {
    l0 = Advance(l0, l0len - l1len);
  } else {
    l1 = Advance(l1, l1len - l0len);
  }
  if (l0 == nullptr) return nullptr;
  if (l1 == nullptr) return nullptr;
  while(l1 != l0) {
    l0 = Advance(l0, 1);
    l1 = Advance(l1, 1);
  }

  return l0;
}

void OverlappingNoCycleListsWrapper(TimedExecutor& executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1", "common"};
  return GenericTestMain(
      args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
      &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
