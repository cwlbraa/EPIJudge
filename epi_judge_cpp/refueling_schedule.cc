#include <vector>
#include <algorithm>
#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
const int kMPG = 20;

struct City {
  int index;
  int gas;
  int distance;
  City(int index, int gas, int distance) : index(index), gas(gas), distance(distance) {}
};

bool Validate(City& city, int remaining_distance, const vector<int>& gallons, const vector<int>& distances) {
  int gas = city.gas - (distances[city.index] / 20);
  remaining_distance -= city.gas * 20;
  for (int i = city.index + 1; i != city.index; i = (i + 1) % gallons.size()) {
    if (gas < 0) return false;
    if (remaining_distance < 0) return true;
    gas += city.gas;
    gas -= distances[city.index] / 20;
    remaining_distance -= city.gas * 20;
  }
  return true;
}

// gallons[i] is the amount of gas in city i, and distances[i] is the distance
// city i to the next city.
int FindAmpleCity(const vector<int>& gallons, const vector<int>& distances) {
  int total_distance = std::accumulate(distances.begin(), distances.end(), 0, std::plus<>());
  vector<City> cities{};
  for (int i = 0; i < gallons.size(); i++) {
    cities.emplace_back(i, gallons[i], distances[i]);
  }

  City min_city = cities[0];
  int min_remaining_gas = 0;
  int remaining_gas = 0;
  for (int i = 1; i < cities.size(); i++) {
    remaining_gas += cities[i-1].gas - cities[i-1].distance / kMPG;
    if (remaining_gas < min_remaining_gas) {
      min_city = cities[i];
      min_remaining_gas = remaining_gas;
    }
  }

  return min_city.index;
}
void FindAmpleCityWrapper(TimedExecutor& executor, const vector<int>& gallons,
                          const vector<int>& distances) {
  int result = executor.Run([&] { return FindAmpleCity(gallons, distances); });
  const int num_cities = gallons.size();
  int tank = 0;
  for (int i = 0; i < num_cities; ++i) {
    int city = (result + i) % num_cities;
    tank += gallons[city] * kMPG - distances[city];
    if (tank < 0) {
      throw TestFailure(FmtStr("Out of gas on city {}", i));
    }
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "gallons", "distances"};
  return GenericTestMain(args, "refueling_schedule.cc",
                         "refueling_schedule.tsv", &FindAmpleCityWrapper,
                         DefaultComparator{}, param_names);
}
