//
//  WaitScene.cpp
//  FristGame
//
//  Created by sfbest on 2017/12/14.
//
//

#include "WaitScene.hpp"
#include "GameScene.hpp"

bool WaitScene::init(GameType type, GamePropModel *model)
{
    Layer::init();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wait_bobm.plist");
    
    Sprite *sp = Sprite::createWithSpriteFrameName("bobm_waiting_01.png");
    sp->setAnchorPoint(Vec2(0.5, 0.5));
    sp->setPosition(kWinSizeWidth * 0.5, kWinSizeHeight * 0.65);
    addChild(sp);
    
    Vector<SpriteFrame *>frames;
    for (int i = 1; i < 7; i++) {
        char name[30];
        sprintf(name, "bobm_waiting_0%d.png", i);
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.15);
    animation->setRestoreOriginalFrame(true);
    auto anim = Animate::create(animation);
    
    sp->runAction(Sequence::create(anim, anim, CallFunc::create([=](){
        Director::getInstance()->replaceScene(Game::createScene(type, model));
    }), NULL));
    
    return true;
}

Scene * WaitScene::createScene(GameType type, GamePropModel *model)
{
    auto scene = Scene::create();
    auto layer = WaitScene::create(type, model);
    scene->addChild(layer);
    
    return scene;
}

WaitScene * WaitScene::create(GameType type, GamePropModel *model)
{
    auto layer = new WaitScene();
    if (layer && layer->init(type, model)) {
        layer->autorelease();
    } else {
        delete layer;
        layer = nullptr;
    }
    
    return layer;
}
