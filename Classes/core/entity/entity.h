#ifndef TOWERDEFENCE_CORE_ENTITY_ENTITY
#define TOWERDEFENCE_CORE_ENTITY_ENTITY

#include <cstdint>
#include <unordered_map>

namespace towerdefence {
namespace core {

struct GridRef;

struct AttackMixin {
    int32_t realized_attack = 0;
};

struct Buff {
    // actual_attack_speed = base_attack_speed + attack_speed
    int32_t attack_speed = 0;
    // actual_speed = base_speed * (1 + speed)
    double speed = 0;
    // actual_attack = base_attack * (1 + attack)
    double attack = 0;
    bool invincible = false;
    bool not_hit = true;
    bool silent = false;
};

struct IdMixin {
    uint32_t id;
};

struct BuffMixin {
    std::unordered_map<uint32_t, Buff> buffs;

    void add_buff(uint32_t id, Buff b) { buffs.insert({id, b}); }
};

struct Entity {
    // called on each tick
    virtual void on_tick(GridRef g) = 0;
    // called when the entity dies
    virtual void on_death(GridRef g);
    // called when entity is hit
    virtual void on_hit(GridRef g);
    virtual ~Entity() = 0;
};

struct Defence {
    int32_t physics = 0;
    int32_t magic = 0;
};

struct EnemyInfo {
    int32_t health = 0;
    Defence defence;
    int32_t speed = 0;
};

struct Enemy : Entity, AttackMixin, BuffMixin, IdMixin {
    virtual EnemyInfo info() const = 0;
};

enum class AttackType { Physics, Magic, Real };

struct TowerInfo {
    int32_t attack = 0;
    int32_t cost = 0;
    int32_t deploy_interval = 0;
    int32_t attack_interval = 0; // actual_attack_attack_speed
    AttackType attack_type;
};

struct Tower : Entity, AttackMixin, BuffMixin, IdMixin {
    virtual TowerInfo info() const = 0;

    void increase_attack(int32_t atk) { realized_attack += atk; }
};

} // namespace core
} // namespace towerdefence

#endif