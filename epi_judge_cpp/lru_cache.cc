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
  list<int>::iterator qspot;
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
 list<int> queue_;

 public:
  LruCache(size_t capacity) : cap_(capacity) {}

  void MoveToNewest(PCItem item) {
    cache_.erase(queue_.back());
    queue_.pop_back();
    Insert(item->isbn, item->price);
  }

  int Lookup(int isbn) {
    if (cache_.find(isbn) == cache_.end()) {
      return -1;
    }

    MoveToNewest(cache_[isbn]);
    return cache_[isbn]->price;
  }

  void Insert(int isbn, int price) {
    if (cache_.find(isbn) != cache_.end()) {
      MoveToNewest(cache_[isbn]);
      return;
    }

    auto item = std::make_shared<CItem>(CItem{isbn, price})
    cache_.insert({isbn, item)});
    queue_.push_front(item);
    item->qspot = queue_.begin();

    if (cache_.size() > cap_) {
      cache_.erase(queue_.back());
      queue_.pop_back();
    }
  }


  bool Erase(int isbn) {
    auto it = cache_.find(isbn);
    if (it == cache_.end()) {
      return false;
    }
    queue_.erase(it->second.qspot);
    cache_.erase(it);
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
