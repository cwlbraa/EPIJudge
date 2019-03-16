#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::deque;
using std::vector;

struct Coord {
  Coord(int x, int y) : x(x), y(y) {}
  int x;
  int y;
  bool operator<(Coord& other) {
    if (x != other.x) {
      return x > other.x;
    }
    return y > other.y;
  }
};

void Contiguous(int x, int y, vector<deque<bool>>* image_ptr) {

  bool target = image_ptr->at(x)[y];
  vector<Coord> nexts = {
    {x+1, y},
    {x, y+1},
    {x-1, y},
    {x, y-1},
  };

  for (auto c : nexts) {
    if (c.x < 0 || c.y < 0
        || c.x >= image_ptr->size()
        || c.y >= image_ptr->at(x).size()) continue;
    if (image_ptr->at(c.x)[c.y] == target) continue;
    image_ptr->at(c.x)[c.y] = target;
    Contiguous(c.x, c.y, image_ptr);
  }
}

void FlipColor(int x, int y, vector<deque<bool>>* image_ptr) {
  image_ptr->at(x)[y] = !image_ptr->at(x)[y];
  Contiguous(x,y,image_ptr);
  return;
}

vector<vector<int>> FlipColorWrapper(TimedExecutor& executor, int x, int y,
                                     vector<vector<int>> image) {
  vector<deque<bool>> b;
  b.reserve(image.size());
  for (const vector<int>& row : image) {
    deque<bool> tmp;
    tmp.resize(row.size());
    for (int i = 0; i < row.size(); ++i) {
      tmp[i] = static_cast<bool>(row[i]);
    }
    b.push_back(tmp);
  }

  executor.Run([&] { FlipColor(x, y, &b); });

  image.resize(b.size());

  for (int i = 0; i < image.size(); ++i) {
    image[i].resize(b.size());
    for (int j = 0; j < image[i].size(); ++j) {
      image[i][j] = b[i][j];
    }
  }
  return image;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "x", "y", "image"};
  return GenericTestMain(args, "matrix_connected_regions.cc", "painting.tsv",
                         &FlipColorWrapper, DefaultComparator{}, param_names);
}
