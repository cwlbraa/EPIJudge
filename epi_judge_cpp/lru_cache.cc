#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include <memory>
#include <unordered_map>
#include <map>
#include <queue>

using std::unordered_map;
using std::map;
using std::shared_ptr;

struct CItem;
using PCItem = shared_ptr<CItem>;

struct CItem {
  int isbn;
  int price;
  PCItem prev;
  PCItem next;

};

std::ostream& operator<<(std::ostream& o, PCItem p) {
  o << "(" << p->isbn <<", " << p->price << ")";
  return o;
}

std::ostream& operator<<(std::ostream& o, unordered_map<int, PCItem> m) {
  o << "[";
  for (auto p : m) o << p.first << ", " << p.second << "; ";
  o << "]";
  return o;
}

class LruCache {
 unordered_map<int, PCItem> cache_{};
 int cap_;
 PCItem oldest_;
 PCItem newest_;

 public:
  LruCache(size_t capacity) : cap_(capacity) {}

  void MoveToNewest_(PCItem item) {
    Remove(item);
    Insert(item->isbn, item->price);
  }

  int Lookup(int isbn) {
    if (cache_.find(isbn) == cache_.end()) {
      return -1;
    }

    MoveToNewest_(cache_[isbn]);
    return cache_[isbn]->price;
  }

  void Insert(int isbn, int price) {
    if (cache_.find(isbn) != cache_.end()) {
      MoveToNewest_(cache_[isbn]);
      return;
    }

    cache_.insert({isbn, std::make_shared<CItem>(CItem{isbn, price, newest_, nullptr})});
    if (oldest_ == nullptr) oldest_ = cache_[isbn];
    if (newest_ != nullptr) newest_->next = cache_[isbn];
    newest_ = cache_[isbn];

    if (cache_.size() > cap_) {
      Remove(oldest_);
    }
  }

  void Remove(PCItem discard) {
    if (discard == nullptr) return;

    auto prev = discard->prev;
    auto next = discard->next;
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (discard == oldest_) oldest_ = discard->next;
    if (discard == newest_) newest_ = newest_->prev;

    cache_.erase(discard->isbn);
  }

  bool Erase(int isbn) {
    if (cache_.find(isbn) == cache_.end()) {
      return false;
    }

    Remove(cache_[isbn]);
    return true;
  }
};
struct Op {
  std::string code;
  int arg1;
  int arg2;
};

template <>
struct SerializationTraits<Op> : UserSerTraits<Op, std::string, int, int> {};

void RunTest(const std::vector<Op>& commands) {
  if (commands.empty() || commands[0].code != "LruCache") {
    throw std::runtime_error("Expected LruCache as first command");
  }
  LruCache cache(commands[0].arg1);

  for (int i = 1; i < commands.size(); i++) {
    auto& cmd = commands[i];
    if (cmd.code == "lookup") {
      int result = cache.Lookup(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Lookup: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else if (cmd.code == "insert") {
      cache.Insert(cmd.arg1, cmd.arg2);
    } else if (cmd.code == "erase") {
      bool result = cache.Erase(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Erase: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else {
      throw std::runtime_error("Unexpected command " + cmd.code);
    }
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"commands"};
  return GenericTestMain(args, "lru_cache.cc", "lru_cache.tsv", &RunTest,
                         DefaultComparator{}, param_names);
}
