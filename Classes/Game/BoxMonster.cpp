//
//  BoxMonster.cpp
//  FristGame
//
//  Created by sfbest on 2017/6/19.
//
//  宝箱怪物

#include "BoxMonster.hpp"
#include "GameScene.hpp"

bool BoxMonster::init()
{
    Layer::init();
    
    auto csb = CSLoader::createNode("BoxManster.csb");
    addChild(csb);
    
    _box = (Sprite *)csb->getChildByTag(35);
    _monster = (Sprite *)csb->getChildByTag(34);
    _ice = (Sprite *)csb->getChildByTag(36);
    _ice->setZOrder(2);
    _graph = (Sprite *)csb->getChildByTag(37);
    _graph->setZOrder(3);
    hasBoxMonster = false;
    
    return true;
}

void BoxMonster::startAppear()
{
    hasBoxMonster = true;
    _lifeCount = 5;
    _type = Knot;
    
    _ice->stopAllActions();
    _box->stopAllActions();
    _monster->stopAllActions();
    _graph->stopAllActions();
    
    _box->setVisible(true);
    _monster->setVisible(true);
    _ice->setVisible(true);
    _graph->setVisible(true);
    
    // 恢复原始位置
    _monster->setPosition(-53.5, 841.53);
    _box->setPosition(-53.57, 777.71);
    _ice->setPosition(-54.4, 778.96);
    _graph->setPosition(-53.57, 773.84);
    
    float moveTime = 20;
    float moveToX = kWinSizeWidth + 50;
    
    auto fly = RepeatForever::create(Help::getAnimate("purple_fly", 4, 0.15));
    auto move = MoveTo::create(moveTime, Vec2(moveToX, _monster->getPosition().y));
    _monster->runAction(fly);
    _monster->runAction(Sequence::create(move, CallFunc::create([&](){
        hasBoxMonster = false;
        _monster->stopAllActions();
    }), NULL));
    
    _box->runAction(MoveTo::create(moveTime, Vec2(moveToX, _box->getPosition().y)));
    _ice->runAction(MoveTo::create(moveTime, Vec2(moveToX, _ice->getPosition().y)));
    _graph->runAction(MoveTo::create(moveTime, Vec2(moveToX, _graph->getPosition().y)));
    
    randBoxType();
}

void BoxMonster::pause()
{
    if (hasBoxMonster) {
        _monster->pause();
        _box->pause();
        _ice->pause();
        _graph->pause();
    } else {
        return;
    }
}

void BoxMonster::resume()
{
    if (hasBoxMonster) {
        _monster->resume();
        _box->resume();
        _ice->resume();
        _graph->resume();
    } else {
        return;
    }
}

void BoxMonster::randBoxType()
{
    // 随机类型
    int tmp = arc4random_uniform(7);
    string image;
    BalloonType tmpType;
    switch (tmp) {
        case 0:
            tmpType = Circle;
            image = "ice_circle.png";
            break;
            
        case 1:
            tmpType = LessThan;
            image = "ice_than.png";
            break;
            
        case 2:
            tmpType = Z;
            image = "ice_z.png";
            break;
            
        case 3:
            tmpType = Lightning;
            image = "ice_lightning.png";
            break;
            
        case 4:
            tmpType = Triangle;
            image = "ice_triangle.png";
            break;
            
        case 5:
            tmpType = Eight;
            image = "ice_eight.png";
            break;
            
        case 6:
            tmpType = Ladder;
            image = "ice_level.png";
            break;
        default:
            break;
    }
    
    while (_type == tmpType) {
        tmp = arc4random_uniform(7);
        switch (tmp) {
            case 0:
                tmpType = Circle;
                image = "ice_circle.png";
                break;
                
            case 1:
                tmpType = LessThan;
                image = "ice_than.png";
                break;
                
            case 2:
                tmpType = Z;
                image = "ice_z.png";
                break;
                
            case 3:
                tmpType = Lightning;
                image = "ice_lightning.png";
                break;
                
            case 4:
                tmpType = Triangle;
                image = "ice_triangle.png";
                break;
                
            case 5:
                tmpType = Eight;
                image = "ice_eight.png";
                break;
                
            case 6:
                tmpType = Ladder;
                image = "ice_level.png";
                break;
            default:
                break;
        }
    }
    
    _type = tmpType;
    
    _graph->setSpriteFrame(image);
}

void BoxMonster::cleanWithType(BalloonType type)
{
    if (!hasBoxMonster) return;
    
    if (type == _type) {
        _lifeCount--;
        if (_lifeCount == 0) {
            startBobm();
        } else {
            startFragmentBobmAnimationWithPosition(_graph->getPosition());
            // 切换图片
            randBoxType();
        }
    }
}

void BoxMonster::startBobm()
{
    hasBoxMonster = false;
    
    _ice->setVisible(false);
    _graph->setVisible(false);
     auto startPosition = _monster->getPosition();
    
    // 宝箱爆炸
    auto *boxAnim = Help::getAnimate(0,"box", 9, 0.05, true);
    _box->runAction(Sequence::create(boxAnim, CallFuncN::create([this](Node *node){
        node->setVisible(false);
        
        // 销毁小鸟
        Animate *anim = Help::getAnimate(0, "purple_die", 2, 0.15, false);;
       
        _monster->runAction(Sequence::create(anim, CallFuncN::create([](Node *node){
            node->setVisible(false);
        }), NULL));
        // 掉落大量金币
        
    }), NULL));
    

    // 消灭monster
    int killNum = arc4random_uniform(100);
    killNum = 21;
    if (killNum > 20 && killNum <= 25 && ((Game *)(this->getParent())->getParent())->_gamePropLayer->_bombCount < 4) {
        // 炸弹
        Sprite *bobm = Sprite::create("prop_bomb_icon.png");
        bobm->setAnchorPoint(Vec2(0.5, 0.5));
        bobm->setPosition(_ice->getPosition());
        bobm->setScale(0.7);
        addChild(bobm);
        
        bobm->runAction(Sequence::create(MoveTo::create(0.8, ((Game *)(this->getParent())->getParent())->_gamePropLayer->_bombBtn->getPosition()), CallFuncN::create([](Node *node){
            node->removeFromParent();
            EventCustom event("killMonster");
            event.setUserData((void *)to_string(1001).c_str());
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }), NULL));
        
    } else if (killNum > 60 && killNum <= 65 && ((Game *)(this->getParent())->getParent())->_gamePropLayer->_decelerationCount < 4) {
        // 减速
        Sprite *bobm = Sprite::create("prop_icon_Deceleration.png");
        bobm->setAnchorPoint(Vec2(0.5, 0.5));
        bobm->setPosition(_ice->getPosition());
        bobm->setScale(0.7);
        addChild(bobm);
        
        bobm->runAction(Sequence::create(MoveTo::create(0.8, ((Game *)(this->getParent())->getParent())->_gamePropLayer->_decelerationBtn->getPosition()), CallFuncN::create([](Node *node){
            node->removeFromParent();
            EventCustom event("killMonster");
            event.setUserData((void *)to_string(1002).c_str());
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }), NULL));
        
    } else {
        killNum = 6;
        
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(-50, 270));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(50, 230));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(20, 250));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(-40, 200));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(0, 200));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(60, 250));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(-60, 250));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(-72, 240));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y - 60), Vec2(72, 240));
        EventCustom event("killMonster");
        event.setUserData((void *)to_string(killNum).c_str());
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void BoxMonster::addOneGoldFall(cocos2d::Vec2 posi, cocos2d::Vec2 offset)
{
    auto *gold1 = Sprite::createWithSpriteFrameName("gold_drow_0.png");
    
    auto anim = Help::getAnimate("gold_drow", 7, 0.1);
    gold1->setPosition(posi);
    gold1->addComponent(PhysicsBody::createBox(gold1->getContentSize()));
    gold1->getPhysicsBody()->setVelocityLimit(1500);
    gold1->getPhysicsBody()->addMoment(-1);
    gold1->getPhysicsBody()->setContactTestBitmask(1);
    gold1->getPhysicsBody()->setVelocity(offset);
    this->addChild(gold1);
    gold1->runAction(RepeatForever::create(anim));
    golds.pushBack(gold1);
    
    if (!_isOpenTimeUp) {
        _isOpenTimeUp = true;
        schedule(schedule_selector(BoxMonster::update), 1);
    }
}

void BoxMonster::update(float delta)
{
    for (int i = 0; i < golds.size(); i++) {
        auto gold = golds.at(i);
        if (gold->getPosition().y <= 0) {
            golds.eraseObject(gold);
            gold->getPhysicsBody()->removeFromWorld();
            gold->removeFromParent();
            gold = nullptr;
        }
    }
    
    if (golds.empty()) {
        unschedule(schedule_selector(BoxMonster::update));
        _isOpenTimeUp = false;
    }
}

void BoxMonster::startFragmentBobmAnimationWithPosition(cocos2d::Vec2 position)
{
    for (int i = 0; i < 10; i++) {
        auto star = Sprite::createWithSpriteFrameName("star_blue.png");
        star->setPosition(Vec2(position.x + 35, position.y - 10));
        star->setScale(0.1);
        addChild(star);
        
        auto rota = RotateTo::create(0.25, 1000);
        auto move = MoveTo::create(0.25, randPosition(position));
        auto scale = ScaleTo::create(0.3, arc4random_uniform(5) / 10.0 + 0.5);
        star->runAction(Spawn::create(rota, move, Sequence::create(scale, CallFuncN::create([](Node *n){
            n->removeFromParent();
        }), NULL), NULL));
    }
}

Vec2 BoxMonster::randPosition(cocos2d::Vec2 position)
{
    int moveX = arc4random_uniform(50) * (arc4random_uniform(2) == 0 ? 1 : -1);
    int moveY = arc4random_uniform(50) * (arc4random_uniform(2) == 0 ? 1 : -1);
    
    return Vec2(position.x + moveX, position.y + moveY);
}
