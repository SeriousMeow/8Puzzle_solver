#include <algorithm>

#include "solver.h"

solver::State::State(uint32_t distance, const solver::FieldType& field,
                     solver::IdType parent)
    : distance_(distance), parent_(parent), field_(field) {
    euristic_ = CalcualteEuristic();
}

solver::State::State(uint32_t distance, const solver::IdType field_id,
                     const solver::IdType parent)
    : distance_(distance),
      parent_(parent),
      field_(solver::FieldFromId(field_id)) {
    euristic_ = CalcualteEuristic();
}

uint32_t solver::State::GetDistance() const { return distance_; }

void solver::State::SetDistance(uint32_t new_distance) {
    distance_ = new_distance;
}

solver::IdType solver::State::GetParent() const { return parent_; }

void solver::State::SetParent(solver::IdType new_parent) {
    parent_ = new_parent;
}

const solver::FieldType& solver::State::GetFieldRef() const { return field_; }

uint32_t solver::State::Weight() const { return distance_ + euristic_; }

std::vector<solver::IdType> solver::State::GetNeighboursIds() const {
    std::vector<solver::IdType> result;
    result.reserve(4);
    // ищем позицию 0
    auto it = std::find(field_.begin(), field_.end(), 0);
    uint32_t empty_index = static_cast<uint32_t>(it - field_.begin());
    uint32_t empty_row = empty_index / WIDTH;
    uint32_t empty_column = empty_index % HEIGHT;
    solver::FieldType current_field = field_;

    // перебираем сдвиги в пустое поле = обмен пустого с кем-то из соседей
    if (empty_row > 0) {
        std::swap(current_field[empty_index],
                  current_field[empty_index - WIDTH]);
        result.push_back(solver::FieldId(current_field));
        std::swap(current_field[empty_index],
                  current_field[empty_index - WIDTH]);
    }
    if (empty_row + 1 < HEIGHT) {
        std::swap(current_field[empty_index],
                  current_field[empty_index + WIDTH]);
        result.push_back(solver::FieldId(current_field));
        std::swap(current_field[empty_index],
                  current_field[empty_index + WIDTH]);
    }
    if (empty_column > 0) {
        std::swap(current_field[empty_index], current_field[empty_index - 1]);
        result.push_back(solver::FieldId(current_field));
        std::swap(current_field[empty_index], current_field[empty_index - 1]);
    }
    if (empty_column + 1 < WIDTH) {
        std::swap(current_field[empty_index], current_field[empty_index + 1]);
        result.push_back(solver::FieldId(current_field));
        std::swap(current_field[empty_index], current_field[empty_index + 1]);
    }
    return result;
}

solver::IdType solver::State::GetId() const { return solver::FieldId(field_); }

uint32_t solver::State::CalcualteEuristic() const {
    // количество клеток не на своей позиции
    uint32_t result = 0;
    for (uint32_t i = 0; i < field_.size(); ++i) {
        if (field_[i] != static_cast<int8_t>((i + 1) % field_.size())) {
            ++result;
        }
    }
    return result;
}
