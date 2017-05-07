#include "StartScean.hpp"
#include "SimpleAudioEngine.h"
//#include "Hand.h"
//#include "Bezier.h"
USING_NS_CC;

Scene* StartScean::createScene()
{
    return StartScean::create();
}

// on "init" you need to initialize your instance
bool StartScean::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(StartScean::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto item1 = MenuItemFont::create( "Test pushScene");
    auto item2 = MenuItemFont::create( "Test pushScene w/transition");
    auto item3 = MenuItemFont::create( "Quit");
    
    auto menu = Menu::create( item1, item2, item3, nullptr );
    menu->alignItemsVertically();
    
    addChild( menu );
    //    this->addChild(sprite, 0);
    
    return true;
}


void StartScean::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
