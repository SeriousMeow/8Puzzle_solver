#include <algorithm>
#include <vector>

#include "solver.h"

solver::IdType solver::FieldId(const solver::FieldType& field) {
    // записывает все числа подряд в десятичное число
    uint32_t hash = 0;
    for (auto element : field) {
        hash *= 10;
        hash += element;
    }
    return hash;
}

solver::FieldType solver::FieldFromId(solver::IdType id) {
    FieldType result;
    for (size_t i = result.size(); i > 0; --i) {
        result[i - 1] = id % 10;
        id /= 10;
    }
    return result;
}

std::vector<solver::IdType> solver::GenerateSolutionIds(
    const SolveResult& solution_result) {
    std::vector<solver::IdType> result;
    solver::IdType current = 123456780;
    while (current != solver::NULLID) {
        result.push_back(current);
        current = (*(solution_result.all_states))[current].GetParent();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

bool solver::IsSolvable(const State& state) {
    uint32_t inversions{0};
    solver::FieldType field = state.GetFieldRef();
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = i + 1; j < field.size(); ++j) {
            if (field[i] == 0 or field[j] == 0) {
                continue;
            }
            if (field[i] > field[j]) {
                ++inversions;
            }
        }
    }
    if (solver::WIDTH % 2 == 1) {
        return inversions % 2 == 0;
    }
    auto it = std::find(field.begin(), field.end(), 0);
    uint32_t empty_index = static_cast<uint32_t>(it - field.begin());
    uint32_t empty_row = empty_index / WIDTH;
    return (empty_row + inversions) % 2 == 0;
}
