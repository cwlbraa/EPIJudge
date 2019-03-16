#include <istream>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
typedef enum { kWhite, kBlack } Color;
struct Coordinate {
  bool operator==(const Coordinate& that) const {
    return x == that.x && y == that.y;
  }
  Coordinate(int x, int y) : x(x), y(y) {}
  int x, y;
};

template<typename T>
std::ostream& operator<<(std::ostream& o, std::vector<T>& v) {
  o << "[";
  for (auto i : v) {
    o << i << ", ";
  }
  o << "]";
  return o;
}
std::ostream& operator<<(std::ostream& o, Coordinate& c) {
  o << "[" << c.x << ", " << c.y << "]";
  return o;
}

bool Search(vector<vector<Color>>& maze,
    const Coordinate& s, const Coordinate& e,
    vector<Coordinate>& result) {

  result.push_back(s);
  maze[s.x][s.y] = kBlack;
  if (s == e) {
    return true;
  }

  vector<Coordinate> nexts = {
    {s.x+1, s.y},
    {s.x, s.y+1},
    {s.x-1, s.y},
    {s.x, s.y-1},
  };

  for (auto c :  nexts) {
    if (c.x < 0
     || c.y < 0
     || c.x >= maze.size()
     || c.y >= maze[0].size()) continue;

    if (maze[c.x][c.y] == kBlack) continue;

    if (Search(maze, c, e, result)) return true;
  }

  result.pop_back();

  return false;
}

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate& s,
                              const Coordinate& e) {
  vector<Coordinate> result{};
  auto success = Search(maze, s, e, result);
  if (success) return result;
  return {};
}


template <>
struct SerializationTraits<Color> : SerializationTraits<int> {
  using serialization_type = Color;

  static serialization_type Parse(const std::string& str) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::Parse(str));
  }

  static serialization_type JsonParse(const json_parser::Json& json_object) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::JsonParse(json_object));
  }
};

template <>
struct SerializationTraits<Coordinate> : UserSerTraits<Coordinate, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string& arg_name) {
    return {};
  }

  static std::vector<int> GetMetrics(const Coordinate& x) { return {}; }
};

bool PathElementIsFeasible(const vector<vector<Color>>& maze,
                           const Coordinate& prev, const Coordinate& cur) {
  if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
        cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == kWhite)) {
    return false;
  }
  return cur == Coordinate{prev.x + 1, prev.y} ||
         cur == Coordinate{prev.x - 1, prev.y} ||
         cur == Coordinate{prev.x, prev.y + 1} ||
         cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor& executor,
                       const vector<vector<Color>>& maze, const Coordinate& s,
                       const Coordinate& e) {
  vector<vector<Color>> copy = maze;

  auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

  if (path.empty()) {
    return s == e;
  }

  if (!(path.front() == s) || !(path.back() == e)) {
    throw TestFailure("Path doesn't lay between start and end points");
  }

  for (size_t i = 1; i < path.size(); i++) {
    if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
      throw TestFailure("Path contains invalid segments");
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "maze", "s", "e"};
  return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                         &SearchMazeWrapper, DefaultComparator{}, param_names);
}
