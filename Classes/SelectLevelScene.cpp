#include "SelectLevelScene.h"
#include "Gif/GIFMovie.h"
#include "Gif/CacheGif.h"
#include "Gif/InstantGif.h"

USING_NS_CC;

Scene* SelectLevelScene::createScene()
{
    return SelectLevelScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SelectLevelScene::init()
{
    if( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(SelectLevelScene::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 100);
    auto background=Sprite::create("images/sellect_background.jpg", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/menu_background_copy.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
        background->setOpacity(0);
        auto fadeIn = FadeIn::create(1.0f);
        background->runAction(fadeIn);
    }
    return true;
}

void SelectLevelScene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}