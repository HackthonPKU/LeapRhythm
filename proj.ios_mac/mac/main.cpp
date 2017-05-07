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
#include <unistd.h>
#include <fstream>
USING_NS_CC;

using namespace Leap;
using namespace std;

bool flag = false;

static bool hit = false;

int64_t pauseCount = 0;

int64_t prevPause = -1;

Frame current;

pthread_mutex_t mutex_k;

Leap::Vector dest;
Leap::Vector mem[4];
float score = 0;

extern Dest dest_global;

int64_t combo = 0;

Frame pauseFrame;
int64_t offset;
bool isPause;

AppDelegate app;

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
struct MusicPoint {
    int64_t t;
    int x, y;
};

void startGame()
{
    pauseCount = 0;
    prevPause = current.timestamp() + 50000;
    flag = true;
    dest_global.start = true;
}

vector<MusicPoint> points;
int scoreUpdate(Frame now,int64_t pauseCount,int64_t timeLenth, int64_t time1)
{
    const HandList hands = now.hands();
    
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
    {
        const Hand hand = *hl;
        Leap::Vector palm = hand.palmPosition();
        float distanceX = palm.x - points[pauseCount].x;
        float distanceY = palm.y - points[pauseCount].y;
        if (sqrt(distanceX*distanceX + distanceY*distanceY) < 20)
        {
            if(time1 - timeLenth > 2*timeLenth)
                score += 1;
            else if(time1 - timeLenth > timeLenth)
                score += 2;
            else
                score += 3;
            score += combo;
            cout << "Score is :" << score <<endl;
            hit = true;
        }
    }
    dest_global.hit = hit;
    dest_global.score = score;
    dest_global.combo = combo;
    return score;
}



void ioread() {
    fstream fin;
    fin.open("Resources/data.txt");
    int64_t sumtime;
    fin >> sumtime;
    MusicPoint mp;
    while (fin >> mp.t)
    {
        fin >> mp.x >> mp.y;
        points.push_back(mp);
    }
    
    fin.close();
    return;
}

int cur_beat = 0;
void SampleListener::onFrame(const Leap::Controller& controller) {
    const Frame frame = controller.frame();
    pthread_mutex_lock(&mutex_k);
    current = frame;
    pthread_mutex_unlock(&mutex_k);
    /*std::cout << "Frame id: " << frame.id()
     << ", timestamp: " << frame.timestamp()
     << ", hands: " << frame.hands().count()
     << ", fingers: " << frame.fingers().count()
     << ", tools: " << frame.tools().count()
     << ", gestures: " << frame.gestures().count() << std::endl;*/
    HandList hands = frame.hands();
    if (frame.hands().count()>1)
        return ;
    bool ges = false;
    if (frame.hands().count()!=0 && flag == false)
    {
        Hand handfirst = *(hands.begin());
        if (handfirst.grabStrength() >= 0.7)
        {
            startGame();
        }
    }
    if (frame.hands().count()!=0)
    {
        Hand handfirst = *(hands.begin());
        if (handfirst.palmNormal().y >= 0)
        {
            ges = true;
        }
        else
        {
            ges = false;
        }
    }
    //if (frame.hands().count()==0||ges)
    if (ges)
    {
        pthread_mutex_lock(&mutex_k);
        pauseFrame = current;
        isPause = true;
        pthread_mutex_unlock(&mutex_k);
    }
    else
    {
        if (isPause == true)
            offset += current.timestamp() - pauseFrame.timestamp();
        pthread_mutex_lock(&mutex_k);
        //pauseFrame = current;
        isPause = false;
        pthread_mutex_unlock(&mutex_k);
    }
    dest_global.ispause = isPause;
    if (isPause)
    {
        if (frame.hands().count()!=0)
        {
            Hand handfirst = *(hands.begin());
            if (handfirst.grabStrength() >= 0.93)
            {
                exit(0);
            }
        }
        return ;
    }
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
    {
        const Hand hand = *hl;
        //bool flag = zeroSpeed(hand);
        
        if (!hit)
            scoreUpdate(frame, pauseCount,frame.timestamp()-offset-prevPause,points[pauseCount+1].t - points[pauseCount].t);
        //const Leap::Hand hand = *hl;
        global.x = hand.palmPosition().x;
        global.y = hand.palmPosition().y;
        global.strength = hand.grabStrength();
    }
    if (flag && frame.timestamp() - offset - prevPause >= points[pauseCount+1].t - points[pauseCount].t)
    {
        pthread_mutex_lock(&mutex_k);
        Frame mycur = frame;
        offset = 0;
        pthread_mutex_unlock(&mutex_k);
        int64_t mytime = mycur.timestamp();
        pthread_mutex_lock(&mutex_k);
        if (hit)
            combo ++;
        else if (points[pauseCount].y > 0)
            combo = 0;
        prevPause = mytime;
        pauseCount ++;
        hit = false;
        pthread_mutex_unlock(&mutex_k);
        if (pauseCount%4 == 0)
            cout << "down" <<endl;
        else if (pauseCount%4 == 1)
            cout << "left" <<endl;
        else if (pauseCount%4 == 2)
            cout << "right" <<endl;
        else
            cout << "up" <<endl;
        if(points[pauseCount].y < 0) return;
        dest_global.changed = true;
        dest_global.x = points[pauseCount].x;
        dest_global.y = points[pauseCount].y;
        printf("Position is:%f %f\n",dest_global.x,dest_global.y);
    }
}

int main(int argc, char *argv[])
{
    ioread();
    isPause = false;
    offset = 0;
     mem[0].x = -70;
    mem[1].x = 0;
    mem[2].x = 0;
    mem[3].x = 70;
    mem[0].y = 180;
    mem[1].y = 130;
    mem[2].y = 250;
    mem[3].y = 180;
    /*pauseCount = 0;
     prevPause = current.timestamp() + 50000;
     flag = true;*/
    SampleListener listener;
    Leap::Controller controller;
    
    
    controller.addListener(listener);
    
    
    return Application::getInstance()->run();
}
