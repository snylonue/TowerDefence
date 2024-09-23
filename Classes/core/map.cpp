#include "map.h"

namespace towerdefence {
namespace core {
void Map::update() {
    for (size_t i = 0; i < shape.height; ++i) {
        for (size_t j = 0; j < shape.width; ++j) {
            auto ref = get_ref(i, j);
            auto &grid = ref.grid;
            if (grid.tower.has_value()) {
                auto &tower = grid.tower.value();
                tower->update(ref);
            }
            for (auto &enemy : grid.enemies) {
                enemy.update(ref);
            }
        }
    }
}

GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
