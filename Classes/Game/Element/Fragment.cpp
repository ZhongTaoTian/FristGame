//
//  Fragment.cpp
//  DefendHomeland
//
//  Created by sfbest on 2017/3/27.
//
//  碎片

#include "Fragment.hpp"

#define kFragmentCount 15

void Fragment::showFragmentToView(cocos2d::Layer *parent, std::vector<Vec2> points, cocos2d::Color4B color)
{
    Vector<Sprite *> fragments;
    auto interval = points.size() / kFragmentCount;
    if (interval == 0) {
        interval = 1;
    }
    for (int i = 0; i < points.size(); i += interval) {
        Sprite *star = Sprite::create("yellow_star.png");
        Vec2 p = points.at(i);
        star->setPosition(p);
        fragments.pushBack(star);
        parent->addChild(star);
        star->setScale((arc4random_uniform(5) / 10.0 + 0.2));
        auto spawnAction = Spawn::create(RotateTo::create(0.5, 1000), ScaleTo::create(0.5, 0),NULL);
        
        auto action = Sequence::create(MoveTo::create(0.2, Vec2(p.x + randPositiveNegativeNumber(30), p.y + randPositiveNegativeNumber(30))), spawnAction, CallFuncN::create([](Node *n){
            n->removeFromParent();
        }), NULL);
        star->runAction(action);
    }
}

float Fragment::randPositiveNegativeNumber(float nubmer)
{
    auto dis = arc4random_uniform(nubmer) + arc4random_uniform(10) / 10.0 + 5;
    auto rand = arc4random_uniform(2);
    if (rand == 0) {
        return dis;
    } else {
        return -dis;
    }
}
