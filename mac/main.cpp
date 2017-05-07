/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "cocos2d.h"
#include "Leap.h"
#include "Hand.h"
USING_NS_CC;

//using namespace Leap;
extern HandData global;
class SampleListener : public Leap::Listener {
public:
    Leap::Frame current;
    virtual void onConnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
};

void SampleListener::onConnect(const Leap::Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

void SampleListener::onFrame(const Leap::Controller& controller) {
    const Leap::Frame frame = controller.frame();
//    pthread_mutex_lock(&mutex_k);
    current = frame;
//    pthread_mutex_unlock(&mutex_k);
    if (frame.hands().count()>2)
        return ;
    Leap::HandList hands = frame.hands();
    for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
    {
        const Leap::Hand hand = *hl;
        global.x = hand.palmPosition().x;
        global.y = hand.palmPosition().y;
        global.strength = hand.grabStrength();
//        HandData::handCenter1 = hand.palmPosition();
    }
}

int main(int argc, char *argv[])
{
    SampleListener listener;
    Leap::Controller controller;

    controller.addListener(listener);
    AppDelegate app;
    return Application::getInstance()->run();
}
