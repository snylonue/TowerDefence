#include "Attack-down.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

void AttackDown::on_tick(GridRef g) {
    for (auto grid : g.with_radius(2, l1_dis)) {
        grid.grid.with_tower([this](std::unique_ptr<Tower> &tower) {
            tower->add_buff({this->id, Buff::DEFAULT}, Buff::attack(-0.5));
        });
    }
}

} // namespace core
} // namespace towerdefence