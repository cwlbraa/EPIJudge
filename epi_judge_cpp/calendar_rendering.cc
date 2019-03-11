#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Event {
  int start, finish;
};

struct Endpoint {
  int t;
  bool is_start;
  bool operator<(Endpoint& other) {
    if (t != other.t) {
      return t < other.t;
    }
    return is_start;
  }
};

int FindMaxSimultaneousEvents(const vector<Event>& A) {
  vector<Endpoint> eps{};
  for (auto e : A) {
    eps.push_back(Endpoint{e.start, true});
    eps.push_back(Endpoint{e.finish, false});
  }
  std::sort(eps.begin(), eps.end());

  int highest = 0;
  int height = 0;
  for (auto& e : eps) {
    if (e.is_start) {
      height++;
    } else {
      height--;
    }
    highest = std::max(height, highest);
  }
  return highest;
}

template <>
struct SerializationTraits<Event> : UserSerTraits<Event, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "calendar_rendering.cc",
                         "calendar_rendering.tsv", &FindMaxSimultaneousEvents,
                         DefaultComparator{}, param_names);
}
