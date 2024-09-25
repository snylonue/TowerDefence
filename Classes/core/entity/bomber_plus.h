#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct bomberPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 2000, .cost = 20, .deploy_interval = 0, .attack_interval = 20, .attack_type = AttackType::Physics
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_PLUS_H