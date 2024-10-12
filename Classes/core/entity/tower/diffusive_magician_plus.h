#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
namespace core {

struct DiffusiveMagicianPlus final : Tower {
    timer::CallbackTimer<DiffusiveMagicianPlus &, GridRef> timeouts_;

    DiffusiveMagicianPlus(id::Id id, const timer::Clock &clk)
        : Tower{id, clk} {}

    static constexpr TowerInfo INFO{2000, 21, 0, 20, 4, AttackType::Magic, TowerType::DiffusiveMagicianPlus};
    TowerInfo info() const override { return INFO; }

    void on_tick(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
