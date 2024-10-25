#include <chrono>
#include <iostream>
#include <random>

#include "solver.h"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 gen(seed);

void RandomShuffle(solver::FieldType& field) {
    for (size_t i = 0; i < field.size(); ++i) {
        std::uniform_int_distribution<> distribution(0, i);
        size_t j = distribution(gen);
        std::swap(field[i], field[j]);
    }
}

int main() {
    const uint32_t total_tests = 100;
    std::cout << "Решение " << total_tests << " случайных позиций" << std::endl;
    uint32_t unsolvable_tests = 0;
    std::vector<size_t> solution_steps;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (uint32_t test_number = 1; test_number <= total_tests; ++test_number) {
        solver::FieldType field{1, 2, 3, 4, 5, 6, 7, 8, 0};
        RandomShuffle(field);

        solver::State start_state = solver::State(0, field, solver::NULLID);

        if (not solver::IsSolvable(start_state)) {
            ++unsolvable_tests;
        } else {
            solver::SolveResult results = solver::Solve(start_state);
            auto solution_ids = solver::GenerateSolutionIds(results);
            solution_steps.push_back(solution_ids.size() - 1);
        }
        if (test_number % 10 == 0) {
            std::cout << test_number << " позиций рассчитано" << std::endl;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "Разрешимо: " << total_tests - unsolvable_tests
              << ", неразрешимо: " << unsolvable_tests << '\n';
    std::cout << "Общее время работы составило " << duration.count()
              << " миллисекунд\n";
    std::cout << "Среднее время обработки позиции "
              << static_cast<double>(duration.count()) /
                     static_cast<double>(total_tests - unsolvable_tests)
              << " миллисекунд\n";
    size_t total_steps =
        std::accumulate(solution_steps.begin(), solution_steps.end(), 0);
    std::cout << "Среднее число шагов "
              << static_cast<double>(total_steps) /
                     static_cast<double>(solution_steps.size());
    std::cout << std::endl;
    return 0;
}
