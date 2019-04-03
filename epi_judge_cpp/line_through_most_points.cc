#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <algorithm>
#include <unordered_map>
#include <string>
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::ostream;

struct Point {
  int x, y;
};

ostream& operator<<(ostream& o, const Point& p) {
  o << "(" << p.x << ", ";
  o << p.y << ")";
  return o;
}

struct Line {
  int mn, md, bn, bd;
  Line(const Point& a, const Point& b) :
    mn( b.y - a.y ),
    md( b.x - a.x ),
    bn( (b.x - a.x) * a.y - (b.y - a.y) * a.x ),
    bd( b.x - a.x )
  {
    // vertical line
    if (md == 0) {
      mn = 1;
      bn = a.x;
      return;
    }
    auto gcm = std::__gcd(mn, md);
    mn /= gcm;
    md /= gcm;
    auto gcb = std::__gcd(bn, bd);
    bn /= gcb;
    bd /= gcb;

    if (md < 0) {
      mn *= -1;
      md *= -1;
    }
    if (bd < 0) {
      bn *= -1;
      bd *= -1;
    }
  }

  bool operator==(const Line& other) const {
    return mn == other.mn
      && md == other.md
      && bn == other.bn
      && bd == other.bd;
  }
};

namespace std {
  template <>
  struct hash<Line> {
    std::size_t operator()(const Line& l) const {
      std::stringstream ss{};
      ss << l.mn << ",";
      ss << l.md << ",";
      ss << l.bn << ",";
      ss << l.bd;
      return std::hash<std::string>()(ss.str());
    }
  };

  template <>
  struct hash<Point> {
    std::size_t operator()(const Point& p) const {
      std::stringstream ss{};
      ss << p.x << ",";
      ss << p.y;
      return std::hash<std::string>()(ss.str());
    }
  };
};

int FindLineWithMostPoints(const vector<Point>& points) {
  unordered_map<Line, unordered_set<Point>> onLines;
  for (int i = 0; i < points.size(); i++) {
    for (int j = i+1; j < points.size(); j++) {
      auto line = Line(points[i], points[j]);
      onLines[line].insert({points[i], points[j]});
    }
  }

  size_t max = 0;
  for (auto& p : onLines) {
    std::cout << p.second;
    max = std::max(max, p.second.size());
  }

  return max;
}

template <>
struct SerializationTraits<Point> : UserSerTraits<Point, int, int> {};

bool operator==(const Point& lhs, const Point& rhs) {
  return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"points"};
  return GenericTestMain(
      args, "line_through_most_points.cc", "line_through_most_points.tsv",
      &FindLineWithMostPoints, DefaultComparator{}, param_names);
}
