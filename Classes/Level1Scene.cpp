#include "Level1Scene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Level1Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(Level1Scene::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto background = Sprite::create("images/select_background.jpg",Rect(0,0,2500,1500));
    if(background == nullptr) {
        problemLoading("'images/select_background.jpg'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }

    //create map
    float delta = 140;
    float x = origin.x + 70 + delta;
    float y = origin.y + visibleSize.height - 70 - delta;
    float SIZE = 140.0;
    ui::Button* grid[7][12]={};
    std::string type[7][12]={};
    type[0][0]=type[0][11]=type[2][11]=type[3][11]=type[4][11]=type[6][0]="out";
    type[2][0]=type[3][0]=type[4][0]=type[6][11]="in";
    type[0][7]=type[6][7]="block_transport";
    type[0][5]=type[0][6]=type[1][0]=type[1][8]=type[1][9]=type[1][10]=type[1][11]=type[5][0]=type[5][7]=type[5][11]=type[6][6]="none";
    type[1][1]=type[1][2]=type[1][3]=type[1][5]=type[1][6]=type[1][7]=type[3][2]=type[4][6]=type[4][7]=type[5][1]=type[5][2]=
    type[5][3]=type[5][4]=type[5][6]=type[5][8]=type[5][9]=type[5][10]="block_high";
    for(int i = 0; i < 7; i++) {
        for (int j = 0; j < 12; j++) {
            if (type[i][j] == "out") {
                grid[i][j] = ui::Button::create("images/out.png","images/out.png");
            }
            else if (type[i][j] == "in") {
                grid[i][j] = ui::Button::create("images/in.png","images/in.png");
            }
            else if (type[i][j] == "block_transport") {
                grid[i][j] = ui::Button::create("images/block_transport.png", "images/block_transport.png");
            }
            else if (type[i][j] == "block_high") {
                grid[i][j] = ui::Button::create("images/block_high.png", "images/block_high.png");
            }
            else if (type[i][j] == "none") {
                grid[i][j] = ui::Button::create();
            }
            else {
                grid[i][j] = ui::Button::create("images/block_low.png", "images/block_low.png");
            }
            grid[i][j]->setPosition(Vec2(x + j * SIZE, y - i * SIZE));
            this->addChild(grid[i][j], 1);
        }
    }
    return true;
}

void Level1Scene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}