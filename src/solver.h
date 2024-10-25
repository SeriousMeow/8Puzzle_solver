#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace solver {

using FieldType = std::array<int8_t, 9>;
using IdType = uint32_t;

const uint32_t HEIGHT = 3;
const uint32_t WIDTH = 3;
const IdType NULLID = 0;

class State {
   public:
    State() {}
    State(uint32_t distance, const FieldType& field, const IdType parent_);
    State(uint32_t distance, const IdType field_id, const IdType parent_);

    uint32_t GetDistance() const;
    void SetDistance(uint32_t new_distance);

    IdType GetParent() const;
    void SetParent(IdType new_parent);

    const FieldType& GetFieldRef() const;

    uint32_t Weight() const;

    std::vector<IdType> GetNeighboursIds() const;

    IdType GetId() const;

   private:
    uint32_t distance_;
    uint32_t euristic_;
    IdType parent_;

    FieldType field_;

    uint32_t CalcualteEuristic() const;
};

struct SolveResult {
    std::shared_ptr<std::unordered_map<IdType, State>> all_states;
};

IdType FieldId(const FieldType& field);

FieldType FieldFromId(IdType id);

SolveResult Solve(const State& start_state);

std::vector<IdType> GenerateSolutionIds(const SolveResult& solution_result);

bool IsSolvable(const State& state);
}  // namespace solver
