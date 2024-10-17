#include "Level2Scene.h"
#include "SelectLevelScene.h"
#include "cocostudio/SimpleAudioEngine.h"

USING_NS_CC;

Scene* Level2Scene::createScene()
{
    return Level2Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level2Scene.cpp\n");
}

bool Level2Scene::init()
{
    LevelScene::init(2);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add bgm
    auto player = CocosDenshion::SimpleAudioEngine::getInstance();
    player->playBackgroundMusic("audio/level2_bgm.MP3", true);

    // the back button to go back to the SelectLevel scene
    auto Back = Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem = MenuItemLabel::create(Back, [this, player](Ref *ref) {
        player->stopBackgroundMusic();
        player->playBackgroundMusic("audio/menu_bgm.MP3", true);
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
    });
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    Vector<MenuItem *> menuItems;
    menuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}