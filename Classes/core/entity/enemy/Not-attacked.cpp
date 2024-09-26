#include "Not-attacked.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

void NotAttacked::on_tick(GridRef g) {}

void NotAttacked::on_hit(GridRef g) {
    if (this->not_hit_) {
        this->add_buff(this->id, Buff::invincible(true));
        this->not_hit_ = false;
    }
}

} // namespace core
} // namespace towerdefence