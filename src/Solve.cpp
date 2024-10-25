#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "solver.h"

solver::SolveResult solver::Solve(const solver::State& start_state) {
    solver::IdType target_id = 123456780;  // полностью решенная головоломка

    std::shared_ptr<std::unordered_map<solver::IdType, solver::State>> states =
        std::make_shared<std::unordered_map<solver::IdType, solver::State>>();

    auto StateIdComparator = [&states](solver::IdType lhs,
                                       solver::IdType rhs) -> bool {
        return (*states)[lhs].Weight() < (*states)[rhs].Weight();
    };

    std::multiset<solver::IdType, decltype(StateIdComparator)> opened_states(
        StateIdComparator);

    std::unordered_set<solver::IdType> closed_states;

    (*states)[start_state.GetId()] = start_state;
    opened_states.insert(start_state.GetId());
    while (not opened_states.empty()) {
        solver::IdType current_state_id = *(opened_states.begin());
        if (current_state_id == target_id) {
            return solver::SolveResult{states};
        }
        opened_states.erase(opened_states.begin());
        closed_states.insert(current_state_id);
        for (solver::IdType neighbour_id :
             (*states)[current_state_id].GetNeighboursIds()) {
            uint32_t distance = (*states)[current_state_id].GetDistance() + 1;
            // если соседа еще нет в памяти, то проинициализируем его
            if (states->count(neighbour_id) == 0) {
                (*states)[neighbour_id] =
                    solver::State(distance, neighbour_id, current_state_id);
                opened_states.insert(neighbour_id);
                continue;
            }

            // не можем улучшить расстояние
            if ((*states)[neighbour_id].GetDistance() <= distance) {
                continue;
            }

            // отсечение закрытых вершин
            if (closed_states.count(neighbour_id) != 0u) {
                continue;
            }

            // можем обновить расстояние
            auto it = opened_states.find(neighbour_id);
            if (it != opened_states.end()) {
                opened_states.erase(it);
            }
            (*states)[neighbour_id].SetDistance(distance);
            (*states)[neighbour_id].SetParent(current_state_id);
            opened_states.insert(current_state_id);
        }
    }
    return solver::SolveResult();
}
