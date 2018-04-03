//
//  Help.cpp
//  FristGame
//
//  Created by sfbest on 2017/5/12.
//
//

#include "Help.hpp"

Animate * Help::getAnimate(string startName, int count, float delayPerUnit)
{
    Vector<SpriteFrame *>frames;
    for (int i = 0; i < count; i++) {
        char name[30];
        sprintf(name, "%s_%d.png", startName.c_str(), i);
        
        if (startName == "eye_blink" && i == 2) {
            continue;
        }
        
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(delayPerUnit);
    animation->setRestoreOriginalFrame(true);
    auto anim = Animate::create(animation);

    return anim;
}

Animate * Help::getAnimate(int startIndex, string startName, int count, float delayPerUnit, bool restoreOriginalFrame)
{
    Vector<SpriteFrame *>frames;
    for (int i = startIndex; i < count; i++) {
        char name[30];
        sprintf(name, "%s_%d.png", startName.c_str(), i);
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(delayPerUnit);
    animation->setRestoreOriginalFrame(restoreOriginalFrame);
    auto anim = Animate::create(animation);
    
    return anim;
}
