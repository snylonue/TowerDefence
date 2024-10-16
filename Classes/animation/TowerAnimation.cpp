//
// Created by Ignorant on 2024/10/12.
//

#include "TowerAnimation.h"

size_t TowerAnimation::removeCounter = 0;

static bool isInRange(int x1, int y1, int x2, int y2, int range) {
    return abs(x1 - x2) + abs(y1 - y2) <= range;
}

Bullet::Bullet(LevelScene *levelScene, towerdefence::core::Tower *tower, towerdefence::core::Enemy *enemy) :
        levelScene(levelScene), tower(tower), enemy(enemy), isScaling(tower->get_storage<bool>("isScaling")) {
    switch (this->tower->status().tower_type_) {
        case TowerType::ArcherBase:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png");
            this->bullet->setScale(0.1f);
            break;
        case TowerType::Archer:
        case TowerType::ArcherPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/archer_arrow.png");
            this->bullet->setScale(0.1f);
            break;
        case TowerType::HighspeedArcher:
        case TowerType::HighspeedArcherPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/highspeed_arrow.png");
            this->bullet->setScale(0.1f);
            break;
        case TowerType::MagicianBase:
        case TowerType::DecelerateMagician:
        case TowerType::DecelerateMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/blueBullet/blueBullet00.png");
            this->bullet->setScale(0.33f);
            break;
        case TowerType::CoreMagician:
        case TowerType::CoreMagicianPlus:
        case TowerType::WeakenMagician:
        case TowerType::WeakenMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/purpleBullet/purpleBullet00.png");
            this->bullet->setScale(0.33f);
            break;
        case TowerType::DiffusiveMagician:
        case TowerType::DiffusiveMagicianPlus:
        case TowerType::HelperBase:
            this->bullet = cocos2d::Sprite::create("images/bullet/greenBullet/greenBullet00.png");
            this->bullet->setScale(0.33f);
            break;
        case TowerType::SpecialMagician:
        case TowerType::SpecialMagicianPlus:
        case TowerType::AggressiveMagician:
        case TowerType::AggressiveMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/orangeBullet/orangeBullet00.png");
            this->bullet->setScale(0.33f);
            break;
        default:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png");
            this->bullet->setScale(0.1f);
            break;
    }
    Id towerId = this->tower->id;
    Id enemyId = this->enemy->id;
    auto *towerSprite = this->levelScene->getTower(towerId);
    float towerX = towerSprite->getPositionX();
    float towerY = towerSprite->getPositionY();
    auto *enemySprite = this->levelScene->getEnemy(enemyId);
    float enemyX = enemySprite->getPositionX();
    float enemyY = enemySprite->getPositionY();
    this->bullet->setPosition(towerX, towerY);
    this->dx = (enemyX - towerX) / this->speed;
    this->dy = (enemyY - towerY) / this->speed;
    if (enemyX == towerX) {
        if (enemyY < towerY) {
            this->angle = 90.0f;
        } else {
            this->angle = -90.0f;
        }
    } else {
        this->angle = (float) (atan((towerY - enemyY) / (enemyX - towerX)) * 180.0f / M_PI);
    }
    this->bullet->setRotation(this->angle);
    tower->set_storage("isScaling", false);
}

void Bullet::explosion() {
    if (this->isScaling) {
        cocos2d::Vector<cocos2d::SpriteFrame *> frames;
        frames.reserve(44);
        for (int i = 0; i < 44; i++) {
            std::string frameName = std::format("images/bullet/bomb/bomb{:02d}.png", i);
            auto *frame = cocos2d::SpriteFrame::create(frameName, cocos2d::Rect(0, 0, 1080, 1080));
            frames.pushBack(frame);
        }
        auto *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
        auto *animate = cocos2d::Animate::create(animation);
        this->bullet->setScale(0.5f);
        this->bullet->runAction(animate);
    } else {
        auto particle = cocos2d::ParticleSystemQuad::create("particles/blood.plist");
        auto enemySprite = this->levelScene->getEnemy(this->enemy->id);
        if (particle) {
            particle->setPosition(enemySprite->getPositionX(), enemySprite->getPositionY());
        }
        this->levelScene->addChild(particle, 4);
    }
}

void Bullet::move() {
    this->updateAngle();
    this->bullet->setRotation(this->angle);
    this->bullet->setPositionX(this->bullet->getPositionX() + this->dx);
    this->bullet->setPositionY(this->bullet->getPositionY() + this->dy);
}

void Bullet::updateAngle() {
    Id enemyId = this->enemy->id;
    float x = this->bullet->getPositionX();
    float y = this->bullet->getPositionY();
    auto *enemySprite = this->levelScene->getEnemy(enemyId);
    float enemyX = enemySprite->getPositionX();
    float enemyY = enemySprite->getPositionY();
    this->bullet->setPosition(x, y);
    this->dx = (enemyX - x) / this->speed;
    this->dy = (enemyY - y) / this->speed;
    if (enemyX == x) {
        if (enemyY < y) {
            this->angle = 90.0f;
        } else {
            this->angle = -90.0f;
        }
    } else {
        this->angle = (float) (atan((y - enemyY) / (enemyX - x)) * 180.0f / M_PI);
    }
}

bool Bullet::isTouch() {
    Id enemyId = this->enemy->id;
    auto *enemySprite = this->levelScene->getEnemy(enemyId);
    float enemyX = enemySprite->getPositionX();
    float enemyY = enemySprite->getPositionY();
    float bulletX = this->bullet->getPositionX();
    float bulletY = this->bullet->getPositionY();
    float distance = sqrt(pow(enemyX - bulletX, 2) + pow(enemyY - bulletY, 2));
    if (distance < 10.0f) {
        return true;
    }
    return false;
}

/* ---------------------- MagicBullet ------------------------ */

MagicBullet::MagicBullet(LevelScene *levelScene, towerdefence::core::Tower *tower, towerdefence::core::Enemy *enemy,
                         std::string color) : Bullet(levelScene, tower, enemy), color(color) {}

void MagicBullet::move() {
    Bullet::move();
    std::string prefix = "images/bullet/" + this->color + "Bullet/" + this->color;
    std::string bulletName = std::format("Bullet{:02d}.png", this->currentFrame);
    this->bullet->setTexture(prefix + bulletName);
    this->currentFrame = (this->currentFrame + 1) % this->totalFrames;
}

void MagicBullet::explosion() {
    this->bullet->setRotation(0);
    if (this->isScaling) {
        this->bullet->setScale(0.5f);
    }
    std::string prefix = "images/bullet/" + this->color + "Bomb/" + this->color;
    this->bullet->setTexture(prefix + "Bomb00.png");
    auto *enemySprite = this->levelScene->getEnemy(this->enemy->id);
    this->bullet->setPosition(enemySprite->getPositionX(), enemySprite->getPositionY());
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    frames.reserve(this->totalFrames);
    for (int i = 0; i < this->totalFrames; i++) {
        std::string frameName = std::format("Bomb{:02d}.png", i);
        auto *frame = cocos2d::SpriteFrame::create(prefix + frameName, cocos2d::Rect(0, 0, 1270, 1270));
        frames.pushBack(frame);
    }
    auto *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto *animate = cocos2d::Animate::create(animation);
    this->bullet->runAction(animate);
}

void TowerAnimation::releaseSkill(LevelScene *levelScene, towerdefence::core::Tower *tower, float duration) {
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    frames.reserve(48);
    for (int i = 0; i < 48; i++) {
        std::string skillPath = std::format("images/bullet/skill/skill{:02d}.png", i);
        auto *frame = cocos2d::SpriteFrame::create(skillPath, cocos2d::Rect(0, 0, 1080, 1080));
        frames.pushBack(frame);
    }
    auto *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto *animate = cocos2d::Animate::create(animation);
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + size;
    float typeY = origin.y + visibleSize.height - size;
    auto towerSprite = levelScene->getTower(tower->id);
    float x = towerSprite->getPositionX();
    float y = towerSprite->getPositionY();
    int indexX = (int) ((x - typeX + 0.5f * size) / size);
    int indexY = (int) ((typeY - y + 0.5f * size) / size);
    
    auto *skillSprite = cocos2d::Sprite::create("images/bullet/skill/skill00.png");
    skillSprite->setPosition(towerSprite->getPositionX(), towerSprite->getPositionY());
    skillSprite->setScale(0.15f);
    skillSprite->setOpacity(50);
    towerSprite->addChild(skillSprite);
    skillSprite->runAction(cocos2d::RepeatForever::create(animate));
    switch (tower->status().tower_type_) {
        case TowerType::HighspeedArcher:
        case TowerType::HighspeedArcherPlus:
        case TowerType::CoreMagician:
            if (duration <= epsilon) {
                towerSprite->removeAllChildren();
            }
            break;
        case TowerType::Bomber:
        case TowerType::BomberPlus:
        case TowerType::SpecialMagician:
            if (duration <= epsilon) {
                towerSprite->removeAllChildren();
            } else if (abs(duration - 1.0f) <= epsilon) {
                tower->set_storage("isScaling", true);
            }
            break;
        case TowerType::ArcherBase:
        case TowerType::Archer:
        case TowerType::ArcherPlus:
        case TowerType::MagicianBase:
        case TowerType::CoreMagicianPlus:
        case TowerType::DiffusiveMagician:
        case TowerType::SpecialMagicianPlus:
        case TowerType::HelperBase:
        case TowerType::DecelerateMagician:
            skillSprite->scheduleOnce([skillSprite](float dt) {
                skillSprite->removeFromParent();
            }, duration, "remove" + std::to_string(TowerAnimation::removeCounter++));
            break;
        case TowerType::DiffusiveMagicianPlus:
            skillSprite->scheduleOnce([skillSprite](float dt) {
                skillSprite->removeFromParent();
            }, duration, "remove" + std::to_string(TowerAnimation::removeCounter++));
            for (int i = indexY - 3; i <= indexY + 3; i++) {
                for (int j = indexX - 3; j <= indexX + 3; j++) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (i >= 0 && i < 7 && j >= 0 && j < 12 && grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create("particles/fire.plist");
                        particle->setPosition(cocos2d::Vec2(typeX + size * j, typeY - size * i));
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce([particle](float dt) {
                            particle->removeFromParent();
                        }, duration, "removeFire" + std::to_string(TowerAnimation::removeCounter++));
                    }
                }
            }
            break;
        case TowerType::WeakenMagician:
        case TowerType::WeakenMagicianPlus:
            skillSprite->scheduleOnce([skillSprite](float dt) {
                skillSprite->removeFromParent();
            }, duration, "remove" + std::to_string(TowerAnimation::removeCounter++));
            for (int i = indexY - 3; i <= indexY + 3; i++) {
                for (int j = indexX - 3; j <= indexX + 3; j++) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (i >= 0 && i < 7 && j >= 0 && j < 12 && grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create("particles/weaken.plist");
                        particle->setPosition(cocos2d::Vec2(typeX + size * j, typeY - size * i));
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce([particle](float dt) {
                            particle->removeFromParent();
                        }, duration, "removeWeaken" + std::to_string(TowerAnimation::removeCounter++));
                    }
                }
            }
            break;
        case TowerType::DecelerateMagicianPlus:
            skillSprite->scheduleOnce([skillSprite](float dt) {
                skillSprite->removeFromParent();
            }, duration, "remove" + std::to_string(TowerAnimation::removeCounter++));
            for (int i = indexY - 3; i <= indexY + 3; i++) {
                for (int j = indexX - 3; j <= indexX + 3; j++) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (i >= 0 && i < 7 && j >= 0 && j < 12 && grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create("particles/decelerate.plist");
                        particle->setPosition(cocos2d::Vec2(typeX + size * j, typeY - size * i));
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce([particle](float dt) {
                            particle->removeFromParent();
                        }, duration, "removeDecelerate" + std::to_string(TowerAnimation::removeCounter++));
                    }
                }
            }
            break;
        case TowerType::AggressiveMagician:
            for (int i = indexY - 3; i <= indexY + 3; i++) {
                for (int j = indexX - 3; j <= indexX + 3; j++) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (i >= 0 && i < 7 && j >= 0 && j < 12 && grid.type == Grid::Type::BlockTower &&
                        isInRange(i, j, indexY, indexX, 3) && i != indexY && j != indexX) {
                        auto attackUp = cocos2d::Sprite::create("images/towers/attack_up.png");
                        attackUp->setPosition(cocos2d::Vec2(typeX + size * j + 40, typeY - size * i));
                        levelScene->addChild(attackUp, 4);
                    }
                }
            }
            break;
        case TowerType::AggressiveMagicianPlus:
            skillSprite->scheduleOnce([skillSprite](float dt) {
                skillSprite->removeFromParent();
            }, duration, "remove" + std::to_string(TowerAnimation::removeCounter++));
            for (int i = indexY - 3; i <= indexY + 3; i++) {
                for (int j = indexX - 3; j <= indexX + 3; j++) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (i >= 0 && i < 7 && j >= 0 && j < 12 && grid.type == Grid::Type::BlockTower &&
                        isInRange(i, j, indexY, indexX, 3) && i != indexY && j != indexX) {
                        auto attackUp = cocos2d::Sprite::create("images/towers/attack_up.png");
                        attackUp->setPosition(cocos2d::Vec2(typeX + size * j + 40, typeY - size * i));
                        levelScene->addChild(attackUp, 4);
                        attackUp->scheduleOnce([attackUp](float dt) {
                            attackUp->removeFromParent();
                        }, duration, "removeAttackUp" + std::to_string(TowerAnimation::removeCounter++));
                    }
                }
            }
            break;
        default:
            break;
    }
}