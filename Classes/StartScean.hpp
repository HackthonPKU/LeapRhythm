//
//  StartScean.hpp
//  Wave
//
//  Created by tegusi on 2017/5/6.
//
//

#ifndef StartScean_hpp
#define StartScean_hpp

#include <stdio.h>

#endif /* StartScean_hpp */

#include "cocos2d.h"

class StartScean : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void myupdate(float delta);
    // implement the "static create()" method manually
    CREATE_FUNC(StartScean);
};
