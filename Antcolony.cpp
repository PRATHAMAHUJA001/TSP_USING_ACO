#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

const int NUM_CITIES = 4;
const int NUM_ANTS = 50;
const int MAX_ITER = 500;
const double ALPHA = 1.0;
const double BETA = 5.0;
const double RHO = 0.7;
const double INITIAL_PHEROMONE = 10.0;

double distances[NUM_CITIES][NUM_CITIES] = {
    {0, 2, 9, 10},
    {1, 0, 6, 4},
    {15, 7, 0, 8},
    {6, 3, 12, 0}
};

double pheromones[NUM_CITIES][NUM_CITIES];

void initialize_pheromones() {
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] = INITIAL_PHEROMONE;
        }
    }
}

bool exists_in_path(int city, const std::vector<int>& path) {
    return std::find(path.begin(), path.end(), city) != path.end();
}

int select_next_city(int current_city, const std::vector<int>& path) {
    std::vector<double> probabilities(NUM_CITIES, 0.0);
    double sum = 0.0;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (!exists_in_path(i, path)) {
            probabilities[i] = pow(pheromones[current_city][i], ALPHA) * pow(1.0 / distances[current_city][i], BETA);
            sum += probabilities[i];
        }
    }

    double rand_value = ((double) rand() / RAND_MAX) * sum;
    double cumulative_prob = 0.0;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (!exists_in_path(i, path)) {
            cumulative_prob += probabilities[i];
            if (rand_value <= cumulative_prob) {
                return i;
            }
        }
    }
    return -1;
}

std::vector<int> construct_solution() {
    std::vector<int> path;
    path.push_back(0);  // starting from the city 0
    while (path.size() < NUM_CITIES) {
        int next_city = select_next_city(path.back(), path);
        path.push_back(next_city);
    }
    return path;
}

double path_length(const std::vector<int>& path) {
    double len = 0.0;
    for (int i = 0; i < path.size() - 1; i++) {
        len += distances[path[i]][path[i + 1]];
    }
    len += distances[path.back()][path[0]];
    return len;
}

void update_pheromones(const std::vector<std::vector<int> >& paths) {
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] *= (1.0 - RHO);
        }
    }

    for (const auto& path : paths) {
        double len = path_length(path);
        for (int i = 0; i < path.size() - 1; i++) {
            pheromones[path[i]][path[i + 1]] += (1.0 / len);
            pheromones[path[i + 1]][path[i]] += (1.0 / len);
        }
        pheromones[path.back()][path[0]] += (1.0 / len);
        pheromones[path[0]][path.back()] += (1.0 / len);
    }
}

int main() {
    srand(time(NULL));
    initialize_pheromones();

    std::vector<int> best_path;
    double best_length = std::numeric_limits<double>::max();

    for (int iter = 0; iter < MAX_ITER; iter++) {
        std::vector<std::vector<int> > paths;

        for (int a = 0; a < NUM_ANTS; a++) {
            auto path = construct_solution();
            double len = path_length(path);
            if (len < best_length) {
                best_length = len;
                best_path = path;
            }
            paths.push_back(path);
        }

        update_pheromones(paths);
    }

    std::cout << "Best path: ";
    for (int city : best_path) {
        std::cout << city << " -> ";
    }
    std::cout << best_path[0] << std::endl;
    std::cout << "Length: " << best_length << std::endl;

    return 0;
}
