#include <chrono>
#include <iostream>

#include "solver.h"

int main() {
    while (true) {
        std::cout << "Ведите позицию (9 цифр подряд построчечно) для решения "
                     "или 0 для выхода"
                  << std::endl;
        solver::IdType start_id;
        std::cin >> start_id;
        if (start_id == solver::NULLID) {
            break;
        }
        solver::State start_state = solver::State(0, start_id, solver::NULLID);

        if (not solver::IsSolvable(start_state)) {
            std::cout << "Позиция: " << start_id << " неразрешима\n";
            continue;
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        solver::SolveResult results = solver::Solve(start_state);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time);
        std::cout << "Время решения позиции " << start_id << " составило "
                  << duration.count() << " миллисекунд\n";

        start_time = std::chrono::high_resolution_clock::now();
        auto solution_ids = solver::GenerateSolutionIds(results);
        end_time = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time);
        std::cout << "Время восстановления решения " << start_id
                  << " составило " << duration.count() << " миллисекунд\n";

        std::cout << "Число операций " << solution_ids.size() - 1 << '\n';

        std::cout << "Решение:\n";
        for (size_t i = 0; i < solution_ids.size(); ++i) {
            std::cout << solution_ids[i];
            if (i + 1 != solution_ids.size()) {
                std::cout << "->";
            }
        }
        std::cout << '\n';
    }
    return 0;
}
