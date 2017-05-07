#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Hand.h"
#include "Bezier.h"
#include <algorithm>
USING_NS_CC;

HandData global;
Dest dest_global;
timeval tm1;
extern list<HitPoint> *l;
extern list<HitPoint> l2, l3, l4;
extern list<HitPoint>::iterator li;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool ispause = true,hit = false,start = false;
void HelloWorld::myupdate(float delta)
{
    if(dest_global.start && !start)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Luv Letter.mp3");
        start = dest_global.start;
    }
    if(ispause != dest_global.ispause)
    {
        if(ispause == false){
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            ispause = dest_global.ispause;}
        else{
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            ispause = dest_global.ispause;}
    }
    auto cursor = (Sprite*) this->getChildByTag(100);
    auto motioin_streak = (Sprite*) this->getChildByTag(101);
    auto width = Director::getInstance()->getVisibleSize().width,
         height = Director::getInstance()->getVisibleSize().height;
    float
    tmp_x = ((global.x + 90) / 180) * width,
    tmp_y = ((global.y - 100) / 180) * height;
    
    //Set up mask layer
    if(dest_global.hit)
    {
        auto mask = Sprite::create("Mask.png");
        Vector<FiniteTimeAction*>acts;
        acts.pushBack(FadeOut::create(0.3f));
        acts.pushBack(RemoveSelf::create());
        auto target = (Sprite*) this->getChildByTag(102);
        if(target!= NULL)
        {
            mask->setPosition(target->getPosition());
            this->addChild(mask);
            target->runAction(RemoveSelf::create());
        }
        mask->runAction(Sequence::create(acts));
    }
    
    //change the position of cursor according to LM
    cursor->setPosition(Vec2(tmp_x,tmp_y));
    cursor->setScale(0.6);
    motioin_streak->setPosition(cursor->getPosition());
}

//get called every 0.1 second
string colors[4] = {"Red","Yellow","Dark Blue","Forgive"};
void HelloWorld::myupdate2(float delta)
{
    auto old_block = (Sprite*) this->getChildByTag(102);
    auto score = (Label*) this->getChildByTag(20);
    auto combo = (Label*) this->getChildByTag(10);
    combo->setString("Combo: " + to_string(dest_global.combo));
    score->setString("Score: " + to_string(dest_global.score));
    auto width = Director::getInstance()->getVisibleSize().width,height = Director::getInstance()->getVisibleSize().height;
    if(!dest_global.changed)
        return;
    
    //Add new block and remove old block
    this->removeChild(old_block, false);
    auto new_node = Sprite::create(colors[(rand() % 4)]+".png");
    float tmp_x = ((dest_global.x + 90) / 180) * width,
    tmp_y = ((dest_global.y - 100) / 180) * height;
    new_node->setPosition(Vec2(tmp_x,tmp_y));
    new_node->setOpacity(0);
    Vector<FiniteTimeAction*>acts;
    acts.pushBack(FadeIn::create(0.4f)); // 0.6 second to fade in
    acts.pushBack(DelayTime::create(0.2f));
//    acts.pushBack(RemoveSelf::create()); // and remove itself
    new_node->runAction(Sequence::create(acts));
    this->addChild(new_node,188,102);
    dest_global.changed = false;
}

//
//void drawLine(Sprite* a)
//{
//    auto draw = DrawNode::create();
//    a->addChild(draw);
//    bezier();
////    for (list<HitPoint>::iterator i = l2.begin();i != l2.end();i++)
////    {
////        draw->drawPoint(Vec2(i->x,i->y), 10, Color4F::GREEN);
////    }
//    for (list<HitPoint>::iterator i = l3.begin();i != l3.end();i++)
//    {
//        float tmp_x = i->x - 150,tmp_y = i->y - 150;
//        draw->drawPoint(Vec2(tmp_x,tmp_y), 10, Color4F::GREEN);
//    }
////    for (list<HitPoint>::iterator i = l4.begin();i != l4.end();i++)
////    {
////        draw->drawPoint(Vec2(i->x,i->y), 10, Color4F::GREEN);
////    }
//}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Luv Letter.mp3");
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto sp = Sprite::create("WechatIMG22.png");
    auto ms = MotionStreak::create(2, 3, 28, Color3B::RED, "steak.png");
    this->addChild(ms,100,101);
    this->addChild(sp,109,100);
    ms->setPosition(sp->getPosition());
    
    //set two timers
    schedule(schedule_selector(HelloWorld::myupdate));
    schedule(schedule_selector(HelloWorld::myupdate2),0.1f);

    //Add two label indicating combo and score
    auto combo = Label::createWithTTF(to_string(dest_global.combo), "fonts/Marker Felt.ttf", 25);
    combo->setPosition(100,150);
    this->addChild(combo,1,10);
    auto score = Label::createWithTTF(to_string(dest_global.score), "fonts/Marker Felt.ttf", 35);
    score->setPosition(100,100);
    this->addChild(score,1,20);
    // position the label on the center of the screen
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
