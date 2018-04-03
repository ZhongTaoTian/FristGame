//
//  MonsterLayer.cpp
//  FristGame
//
//  Created by sfbest on 2017/4/18.
//
//  天空掉落怪物的Layer

#include "MonsterLayer.hpp"


bool MonsterLayer::init(GameType gameType)
{
    Layer::init();
    _gameType = gameType;
    
    _boxManster = BoxMonster::create();
    addChild(_boxManster);
    
    return true;
}



MonsterLayer * MonsterLayer::create(GameType gameType)
{
    auto layer = new MonsterLayer();
    if (layer && layer->init(gameType)) {
        layer->autorelease();
    } else {
        delete layer;
        layer = nullptr;
    }
    
    return layer;
}

// 开始出现
void MonsterLayer::startAppearMonster()
{
    _boxMonsterTime = 0;
    _secondCount = 0;
    
    if (_gameType == Guard) {
        _speed = 0.5;
    } else {
        _speed = 1;
    }
    
    _boxMonsterIndex = arc4random_uniform(600) + 200;
    schedule(schedule_selector(MonsterLayer::update), 1 / 60.0);
}

// 刷新时间
void MonsterLayer::update(float dt)
{
    if (_gameType == Guard && !_isDeceleration) {
        _secondCount++;
        if (_secondCount == 1800) {
            _secondCount = 0;
            
            // 怪物掉落加速
            if (_speed <= 3) {
                _speed += 0.1;
            }
        }
    }
    
    if (_isDeceleration) {
        _decelerationCount++;
        
        if (_decelerationCount == 1200) {
            _isDeceleration = false;
            _decelerationCount = 0;
            _speed = _lastSpeed;
        }
    }
    
    if (!(_boxManster->hasBoxMonster)) {
        _boxMonsterTime++;
        
        if (_boxMonsterTime == _boxMonsterIndex) {
            _boxMonsterTime = 0;
            _boxManster->startAppear();
            
            _boxMonsterIndex = arc4random_uniform(600) + 200;
        }
    }
    if (golds.size() > 0) {
        for (int i = 0; i < golds.size(); i++) {
            auto gold = golds.at(i);
            if (gold->getPosition().y <= 0) {
                golds.eraseObject(gold);
                gold->getPhysicsBody()->removeFromWorld();
                gold->removeFromParent();
                gold = nullptr;
            }
        }
    }
    
    if (_timeIndex == 0) {
        // 随机生成出现间隔
        _apperInterval = arc4random_uniform(80) + 120;
    }
    
    _timeIndex++;
    
    if (_monsters.size() == 0 && _timeIndex == 1) {
        _apperInterval = 1;
    }
    
    if (_timeIndex == _apperInterval) {
        _timeIndex = 0;
        
        // 随机出现一个非Boss类型的Monster
        MonsterBalloonType type = MonsterBalloonType(arc4random_uniform(3));
        Monster *monster = Monster::create(type);
        monster->setAnchorPoint(Vec2(0.5, 0));
        // 随机x,并且判断距离,太近重新生成
        
        float x = arc4random_uniform(kWinSizeWidth - monster->getContentSize().width);
        while (abs(x - _lastMonsterX) < 150) {
            x = arc4random_uniform(kWinSizeWidth - monster->getContentSize().width);
        }
        if (x < 40) {
            x = 40;
        }
        
        if (x > kWinSizeWidth - 100) {
            x = kWinSizeWidth - 100;
        }
        
        _lastMonsterX = x;
        
        monster->setPosition(Vec2(x, kWinSizeHeight));
        _monsters.pushBack(monster);
        addChild(monster);
    }
    
    for (int i = 0; i < _monsters.size(); i++) {
        // 向下降落
        auto monster = _monsters.at(i);
        // 气球多的降落满 少的降落快
        float speed = _speed;
        switch (monster->_type) {
            case OneBalloon:
                speed = speed * 1.5;
                break;
            case TwoBalloon:
                speed = speed * 1.2;
                break;
            case ThreeBalloon:
                break;
            default:
                break;
        }
        monster->setPosition(Vec2(monster->getPosition().x, monster->getPosition().y - speed));
        
        if (!(monster->_isInScreen) && monster->getPosition().y < kWinSizeHeight - monster->getContentSize().height * 0.5) {
            monster->_isInScreen = true;
        }
        
        // 计算是否有落地的Monster
        if (monster->getPosition().y <= 0) {
            // 漏掉的计数+1
            landCountAdd();
            
            _monsters.erase(i);
            monster->removeFromParent();
        }
    }
}

void MonsterLayer::pause()
{
    _boxManster->pause();
    Layer::pause();
}

void MonsterLayer::resume()
{
    _boxManster->resume();
    Layer::resume();
}

// 漏掉的计数+1
void MonsterLayer::landCountAdd()
{
    // 守护模式 生命-1
    if (_gameType == GameType::Guard) {
        EventCustom event("PassMonster");
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

/// 使用道具
void MonsterLayer::useProp(bool isBomb)
{
    if (isBomb) {
        auto bomb = Sprite::create("bomb_effect_icom.png");
        bomb->setAnchorPoint(Vec2(0.5, 0.5));
        bomb->setPosition(kWinSizeWidth * 0.5, kWinSizeHeight * 0.5);
        bomb->setOpacity(200);
        addChild(bomb);
        
        auto whiteLayer = LayerColor::create(Color4B::WHITE);
        whiteLayer->setOpacity(220);
        addChild(whiteLayer, 2);
        
        whiteLayer->runAction(Sequence::create(FadeTo::create(1 / 12.0, 100), CallFuncN::create([](Node *node){
            node->setOpacity(255);
        }),FadeTo::create(1 / 5.0, 0), NULL));
        bomb->runAction(Spawn::create(ScaleTo::create(0.5, 16), Sequence::create(DelayTime::create(0.2), CallFunc::create([&](){
            for (int i = ((int)_monsters.size() - 1); i >= 0; i--) {
                auto monster = _monsters.at(i);
                // 执行掉落动画,完成后自我销毁
                monster->bombAnim();
                // 掉落金币
                fallGold(false, Vec2(monster->getPosition().x + 30, monster->getPosition().y + 10));
                _monsters.eraseObject(monster);
            
                // 消灭monster
                EventCustom event("killMonster");
                
                event.setUserData((void *)to_string(1).c_str());
                    
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                
            }
            
            if (_boxManster && _boxManster->hasBoxMonster) {
                _boxManster->startBobm();
            }
            
        }), NULL) ,Sequence::create(DelayTime::create(0.4), FadeTo::create(0.1, 0), NULL),NULL));
        
        this->getParent()->setOpacity(0.5);
        
        
        
    } else {
        // 减速
        if (_lastSpeed != 0) {
            _speed = _lastSpeed;
        }
        _decelerationCount = 0;
        _isDeceleration = true;
        _lastSpeed = _speed;
        _speed = _speed * 0.5;
    }
}

// 消除绘制形状的气球
void MonsterLayer::removeBalloonWithBalloonType(BalloonType type)
{
    _boxManster->cleanWithType(type);
    if (type == BalloonType::UnKnow || _monsters.size() <= 0) return;
    
    int killNum = 0;
    // 消除宝箱怪物
    
    for (int i = ((int)_monsters.size() - 1); i >= 0; i--) {
        auto monster = _monsters.at(i);
        
        if (!(monster->_isInScreen)) continue;
        
        monster->removeBalloon(type);
        
        if (monster->_balloons.size() <= 0) {
            // 执行掉落动画,完成后自我销毁
            monster->dieAnimation();
            killNum++;
            
            // 掉落金币
            if (i == 0 && killNum > 1) {
                fallGold(true, Vec2(monster->getPosition().x + 30, monster->getPosition().y + 10));
            } else {
                fallGold(false, Vec2(monster->getPosition().x + 30, monster->getPosition().y + 10));
            }
            
            _monsters.eraseObject(monster);
        }
        
        if (i == 0 && killNum > 0) {
            // 消灭monster
            EventCustom event("killMonster");
            
            event.setUserData((void *)to_string(killNum).c_str());
            
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
}

// 掉落金币
void MonsterLayer::fallGold(bool more, cocos2d::Vec2 startPosition)
{
    // 掉落金币
    if (!more) {
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(-50, 270));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(50, 230));
    } else {
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(-50, 270));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(50, 230));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(20, 250));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(-40, 200));
        addOneGoldFall(Vec2(startPosition.x + 12, startPosition.y + 50), Vec2(0, 200));
    }
}

void MonsterLayer::addOneGoldFall(cocos2d::Vec2 posi, cocos2d::Vec2 offset)
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
}
