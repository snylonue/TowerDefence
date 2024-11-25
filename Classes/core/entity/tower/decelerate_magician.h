#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct DecelerateMagician final : Tower, TimeOutMixin<DecelerateMagician> {
        timer::Timer release_skill_;
        bool skill_ = false;

        DecelerateMagician(id::Id id, const timer::Clock & clk);
        DecelerateMagician() = delete;

        static constexpr TowerInfo INFO { 400, 9, 0, 19, 2, AttackType::Magic, TowerType::DecelerateMagician};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_H
