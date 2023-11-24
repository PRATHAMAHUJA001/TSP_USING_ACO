#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <map>

const int NUM_CITIES = 5;
double distances[NUM_CITIES][NUM_CITIES] = {
    {0, 2, 9, 10},
    {1, 0, 6, 4},
    {15, 7, 0, 8},
    {6, 3, 12, 0}
};

std::map<std::pair<int, std::string>, double> memo;

double tsp(int start, std::string& visited) {
    if (visited.find('0') == std::string::npos) { // All cities are visited
        return distances[start][0];
    }

    std::pair<int, std::string> key = std::make_pair(start, visited);

    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    double min_dist = std::numeric_limits<double>::max();
    for (int i = 0; i < NUM_CITIES; i++) {
        if (i != start && visited[i] == '0') {
            visited[i] = '1';
            double dist = distances[start][i] + tsp(i, visited);
            if (dist < min_dist) {
                min_dist = dist;
            }
            visited[i] = '0';
        }
    }

    memo[key] = min_dist;
    return min_dist;
}

int main() {
    std::string visited(NUM_CITIES, '0');
    visited[0] = '1'; // Starting from city 0
    double result = tsp(0, visited);
    std::cout << "Shortest path length: " << result << std::endl;
    return 0;
}
