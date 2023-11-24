#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <limits>

const int NUM_CITIES = 4;
double distances[NUM_CITIES][NUM_CITIES] = {
    {0, 2, 9, 10},
    {1, 0, 6, 4},
    {15, 7, 0, 8},
    {6, 3, 12, 0}
};

using city_set = std::bitset<NUM_CITIES>;
using memo_key = std::pair<int, city_set>;

struct KeyHash {
    std::size_t operator()(const memo_key& k) const {
        return std::hash<int>()(k.first) ^ std::hash<unsigned long long>()(k.second.to_ullong());
    }
};

std::unordered_map<memo_key, double, KeyHash> memo;

double tsp(int start, city_set visited) {
    if (visited.count() == NUM_CITIES) {
        return distances[start][0];
    }

    memo_key key(start, visited);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    double min_dist = std::numeric_limits<double>::max();
    for (int i = 0; i < NUM_CITIES; i++) {
        if (!visited[i]) {
            city_set new_visited = visited;
            new_visited[i] = true;
            double dist = distances[start][i] + tsp(i, new_visited);
            if (dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    memo[key] = min_dist;
    return min_dist;
}

int main() {
    city_set initial_set;
    initial_set[0] = true;
    double result = tsp(0, initial_set);
    std::cout << "Shortest path length: " << result << std::endl;
    return 0;
}